#include "Bloom.h"

#include <DirectXMath.h>

#include "AccessFlag.h"
#include "OrthographicCamera.h"
#include "AlignedNew.h"

#include "BloomExtractPS.h"
#include "GaussianBlurPS.h"
#include "BloomCombinePS.h"

using namespace DirectX;

Bloom::Bloom(GraphicsContext *context)
	: context(context), sceneRenderTexture(context), renderTexture1(context), renderTexture2(context)
{
	bloomExtractPS = context->createPixelShader(g_BloomExtractPS, sizeof(g_BloomExtractPS));
	bloomExtractCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);

	gaussianBlurPS = context->createPixelShader(g_GaussianBlurPS, sizeof(g_GaussianBlurPS));
	gaussianBlurCB = context->createBuffer(BufferType_ConstantBuffer, 480, AccessFlag_Write, nullptr);

	bloomCombinePS = context->createPixelShader(g_BloomCombinePS, sizeof(g_BloomCombinePS));
	bloomCombineCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);
}

Bloom::~Bloom()
{
	context->releasePixelShader(bloomExtractPS);
	context->releaseBuffer(bloomExtractCB);
	context->releasePixelShader(gaussianBlurPS);
	context->releaseBuffer(gaussianBlurCB);
	context->releasePixelShader(bloomCombinePS);
	context->releaseBuffer(bloomCombineCB);
}

void Bloom::apply(SpriteBatch &batch) const
{
	if (!sceneRenderTarget)
		return;
	
	context->setDepthStencilState(context->DSSNone);

	// Copy the content of our source render target to our custom buffer.
	hTexture2D surface = context->getTexture2D(sceneRenderTarget, 0);
	context->copyTexture2D(surface, sceneRenderTexture.getTexture2D());
	context->releaseTexture2D(surface);

	// Pass 1: draw the scene into rendertarget 1, using a
	// shader that extracts only the brightest parts of the image.
	MapData mapData;
	context->mapBuffer(bloomExtractCB, MapType_Write, &mapData);
	float *bloomExtractBuffer = (float*)mapData.mem;
	*bloomExtractBuffer = settings.bloomThreshold;
	context->unmapBuffer(bloomExtractCB);
	context->setPSConstantBuffers(&bloomExtractCB, 0, 1);

	renderFullscreenQuad(batch, renderTexture1.getRenderTarget(), sceneRenderTexture.getTexture2D(), bloomExtractPS, PreBloom);

	// Pass 2: draw from rendertarget 1 into rendertarget 2,
	// using a shader to apply a horizontal gaussian blur filter.
	TextureSize rt1Size;
	context->getTexture2DSize(renderTexture1.getTexture2D(), &rt1Size);

	setBlurParameters(1.0f / (float)rt1Size.width, 0.0f);
	renderFullscreenQuad(batch, renderTexture2.getRenderTarget(), renderTexture1.getTexture2D(), gaussianBlurPS, BlurredHorizontally);

	// Pass 3: draw from rendertarget 2 back into rendertarget 1,
	// using a shader to apply a vertical gaussian blur filter.
	setBlurParameters(0.0f, 1.0f / (float)rt1Size.height);
	renderFullscreenQuad(batch, renderTexture1.getRenderTarget(), renderTexture2.getTexture2D(), gaussianBlurPS, BlurredBothWays);

	// Pass 4: draw both rendertarget 1 and the original scene
	// image back into the main backbuffer, using a shader that
	// combines them to produce the final bloomed result.
	context->mapBuffer(bloomCombineCB, MapType_Write, &mapData);
	float *bloomCombineBuffer = (float*)mapData.mem;
	bloomCombineBuffer[0] = settings.bloomIntensity;
	bloomCombineBuffer[1] = settings.baseIntensity;
	bloomCombineBuffer[2] = settings.bloomSaturation;
	bloomCombineBuffer[3] = settings.baseSaturation;
	context->unmapBuffer(bloomCombineCB);
	context->setPSConstantBuffers(&bloomCombineCB, 0, 1);
	context->setPSTexture2Ds(&sceneRenderTexture.getTexture2D(), 1, 1);

	renderFullscreenQuad(batch, sceneRenderTarget, renderTexture1.getTexture2D(), bloomCombinePS, FinalResult);

	context->setDepthStencilState(context->DSSDefault);
}

void Bloom::renderFullscreenQuad(SpriteBatch &batch, hRenderTarget renderTarget, hTexture2D texture, hPixelShader pixelShader, ShowFilter currentShowFilter) const
{
	// If the user has selected one of the show intermediate buffer options,
	// we still draw the quad to make sure the image will end up on the screen,
	// but might need to skip applying the custom pixel shader.
	if (showFilter < currentShowFilter)
		pixelShader = nullptr;

	TextureSize surfSize;
	context->getRenderTargetSize(renderTarget, 0, &surfSize);

	TextureSize texSize;
	context->getTexture2DSize(texture, &texSize);

	Sprite sprite;
	sprite.setPosition(0, 0);
	sprite.setSize((float)surfSize.width, (float)surfSize.height);
	sprite.setSrcRect(FloatRect(0, 0, (float)texSize.width, (float)texSize.height));
	sprite.setTexture(texture);

	context->setBlendState(context->BSOpaque);
	batch.begin(renderTarget, SpriteSortMode_Deferred, nullptr, nullptr, pixelShader);
	batch.draw(sprite);
	batch.end();
	context->setBlendState(context->BSAlphaBlend);
}

void Bloom::setBlurParameters(float dx, float dy) const
{
	static const uint32_t SampleCount = 15;

	ALIGN(16)
	struct FloatA : AlignedNew<16>
	{
		float value;

		FloatA() { this->value = 0; }
		FloatA(float value) { this->value = value; }
		operator float() { return value; }
	};

	// Create temporary arrays for computing our filter settings.
	XMFLOAT2A sampleOffsets[SampleCount];
	FloatA    sampleWeights[SampleCount];

	// The first sample always has a zero offset.
	sampleOffsets[0] = XMFLOAT2A(0, 0);
	sampleWeights[0] = computeGaussian(0);

	// Maintain a sum of all the weighting values.
	float totalWeights = sampleWeights[0];

	// Add pairs of additional sample taps, positioned
	// along a line in both directions from the center.
	for (uint32_t i = 0; i < SampleCount / 2; i++)
	{
		// Store weights for the positive and negative taps.
		float weight = computeGaussian(i + 1.0f);

		sampleWeights[i * 2 + 1] = weight;
		sampleWeights[i * 2 + 2] = weight;

		totalWeights += weight * 2;

		// To get the maximum amount of blurring from a limited number of
		// pixel shader samples, we take advantage of the bilinear filtering
		// hardware inside the texture fetch unit. If we position our texture
		// coordinates exactly halfway between two texels, the filtering unit
		// will average them for us, giving two samples for the price of one.
		// This allows us to step in units of two texels per sample, rather
		// than just one at a time. The 1.5 offset kicks things off by
		// positioning us nicely in between two texels.
		float sampleOffset = i * 2 + 1.5f;

		XMFLOAT2A delta(dx, dy);
		delta.x *= sampleOffset;
		delta.y *= sampleOffset;

		// Store texture coordinate offsets for the positive and negative taps.
		sampleOffsets[i * 2 + 1] = delta;
		sampleOffsets[i * 2 + 2] = XMFLOAT2A(-delta.x, -delta.y);
	}

	// Normalize the list of sample weightings, so they will always sum to one.
	for (uint32_t i = 0; i < SampleCount; i++)
		sampleWeights[i] = sampleWeights[i] / totalWeights;

	// Tell the shader about our new filter settings.
	MapData mapData;
	context->mapBuffer(gaussianBlurCB, MapType_Write, &mapData);

	XMFLOAT2A *sampleOffsetsBuffer = (XMFLOAT2A*)mapData.mem;
	
	for (uint32_t i = 0; i < SampleCount; i++)
		sampleOffsetsBuffer[i] = sampleOffsets[i];

	FloatA *sampleWeightsBuffer = (FloatA*)(mapData.mem) + SampleCount;

	for (uint32_t i = 0; i < SampleCount; i++)
		sampleWeightsBuffer[i] = sampleWeights[i];

	context->unmapBuffer(gaussianBlurCB);
	context->setPSConstantBuffers(&gaussianBlurCB, 0, 1);
}

float Bloom::computeGaussian(float n) const
{
	float theta = settings.blurAmount;
	
	// sqrt(2 * pi) = 2.50662827463
	return (float)((1.0 / (2.50662827463 * theta)) * std::exp(-(n * n) / (2.0 * theta * theta)));
}

void Bloom::setRenderTarget(hRenderTarget renderTarget)
{
	sceneRenderTarget = renderTarget;

	if (!sceneRenderTarget)
		return;

	// Look up the resolution of our source render target.
	TextureSize rtSize;
	context->getRenderTargetSize(renderTarget, 0, &rtSize);

	if (sceneSize.width != rtSize.width || sceneSize.height != rtSize.height)
	{
		sceneSize = rtSize;

		// Create a texture for rendering the main scene, prior to applying bloom.
		sceneRenderTexture.create(sceneSize.width, sceneSize.height);

		// Create two rendertargets for the bloom processing. These are half the
		// size of the backbuffer, in order to minimize fillrate costs. Reducing
		// the resolution in this way doesn't hurt quality, because we are going
		// to be blurring the bloom images in any case.
		renderTexture1.create(sceneSize.width / 2, sceneSize.height / 2);
		renderTexture2.create(sceneSize.width / 2, sceneSize.height / 2);
	}
}