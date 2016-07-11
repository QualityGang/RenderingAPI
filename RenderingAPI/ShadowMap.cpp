#include "ShadowMap.h"
#include "ComputeDistancesPS.h"
#include "DistortPS.h"
#include "HorizontalReductionPS.h"
#include "HorizontalReductionVS.h"
#include "DrawShadowPS.h"
#include "GaussianBlurPS.h"

ShadowMap::ShadowMap(GraphicsContext* context, ShadowMapSize size) :
	context(context), distancesRT(context), distortRT(context), shadowMapRT(context), shadowsRT(context), processedShadowRT(context), sceneRenderTexture(context)
{
	shadowMapSize = 2 << size;
	reductionChainCount = size;

	// Initialize All Pixel Shaders and Constant Buffers
	computeDistancesPS = context->createPixelShader(g_ComputeDistancesPS, sizeof(g_ComputeDistancesPS));
	computeDistancesCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);

	distortPS = context->createPixelShader(g_DistortPS, sizeof(g_DistortPS));

	hReductionPS = context->createPixelShader(g_HorizontalReductionPS, sizeof(g_HorizontalReductionPS));
	hReductionCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);

	gaussianBlurPS = context->createPixelShader(g_GaussianBlurPS, sizeof(g_GaussianBlurPS));
	gaussianBlurCB = context->createBuffer(BufferType_ConstantBuffer, 480, AccessFlag_Write, nullptr);

	for (int i = 0; i < reductionChainCount; ++i)
	{
		reductionRT.push_back(new RenderTexture(context));
	}

	shadowPS = context->createPixelShader(g_DrawShadowPS, sizeof(g_DrawShadowPS));

	// Create render Textures
	distancesRT.create(shadowMapSize, shadowMapSize, PixelFormat_RG16F);
	distortRT.create(shadowMapSize, shadowMapSize, PixelFormat_RG16F);
	for (int i = 0; i < reductionRT.size(); ++i)
	{
		reductionRT[i]->create(2 << i, shadowMapSize, PixelFormat_RG16F);
	}

	shadowMapRT.create(shadowMapSize, shadowMapSize);
	processedShadowRT.create(shadowMapSize, shadowMapSize); 
	shadowsRT.create(shadowMapSize, shadowMapSize);
}

ShadowMap::~ShadowMap()
{
	// Release

	context->releasePixelShader(computeDistancesPS);
	context->releaseBuffer(computeDistancesCB);

	context->releasePixelShader(distortPS);

	context->releasePixelShader(hReductionPS);
	context->releaseBuffer(hReductionCB);

	context->releasePixelShader(shadowPS);

	context->releasePixelShader(gaussianBlurPS);
	context->releaseBuffer(gaussianBlurCB);

	for (auto rt : reductionRT)
	{
		delete rt;
	}
}

void ShadowMap::setRenderTarget(hRenderTarget renderTarget)
{
	sceneRenderTarget = renderTarget;

	if (!sceneRenderTarget)
		return;

	TextureSize rtSize;
	context->getRenderTargetSize(renderTarget, 0, &rtSize);

	if (size.width != rtSize.width || size.height != rtSize.height)
	{
		size = rtSize;

		sceneRenderTexture.create(size.width, size.height);
	}
}

void ShadowMap::ApplyReduction(SpriteBatch& batch, RenderTexture* source, RenderTexture* destination)
{
	int step = reductionChainCount - 1;

	// Since the Distances of the pixels are stored in the Distorted Texture we don't want colors to mix
	context->setPSSamplers(&context->SSPointClamp, 0, 1);

	while (step >= 0)
	{
		TextureSize dsize;
		context->getTexture2DSize(source->getTexture2D(), &dsize);

		MapData reductionMapData;
		context->mapBuffer(hReductionCB, MapType_Write, &reductionMapData);
		float *tDims = (float*)reductionMapData.mem;
		tDims[0] = 1.0f / (float)dsize.width;
		tDims[1] = 1.0f / (float)dsize.height;
		context->unmapBuffer(hReductionCB);
		context->setPSConstantBuffers(&hReductionCB, 0, 1);

		// Write the Prev Level Reduction to the Next Reduction Render Texture and Apply hReduction on It
		renderFullscreenQuad(batch, reductionRT[step]->getRenderTarget(), source->getTexture2D(), nullptr, hReductionPS, 255);

		source = reductionRT[step];

		step--;
	}		
	
	// Render the last Level Reduction texture (2 x ShadowMapSize Texture) to the ShadowMap Render Texture
	renderFullscreenQuad(batch, destination->getRenderTarget(), reductionRT[0]->getTexture2D(), nullptr, nullptr, 255);

	// Set the Sampler to normal
	context->setPSSamplers(&context->SSLinearClamp, 0, 1);
}

void ShadowMap::draw(SpriteBatch &batch, Color backgroundColor)
{
	if (!sceneRenderTarget)
		return;

	hTexture2D surface = context->getTexture2D(sceneRenderTarget, 0);
	context->copyTexture2D(surface, sceneRenderTexture.getTexture2D());
	context->releaseTexture2D(surface);

	context->setDepthStencilState(context->DSSNone);

	MapData distancesMapData;
	context->mapBuffer(computeDistancesCB, MapType_Write, &distancesMapData);
	float *bgColor = (float*)distancesMapData.mem;
	bgColor[0] = (float)backgroundColor.r / 255.0f;
	bgColor[1] = (float)backgroundColor.g / 255.0f;
	bgColor[2] = (float)backgroundColor.b / 255.0f;
	bgColor[3] = (float)backgroundColor.a / 255.0f;
	context->unmapBuffer(computeDistancesCB);
	context->setPSConstantBuffers(&computeDistancesCB, 0, 1);

	// Apply "ComputeDistancesPS" shader to Scene RenderTexture and Write it into "Distances" Render Texture
	renderFullscreenQuad(batch, distancesRT.getRenderTarget(), sceneRenderTexture.getTexture2D(), nullptr, computeDistancesPS, 255);

	// Apply "DistortPS" shader to "Distances" Render Texture and Write it into "Distort" Render Texture
	renderFullscreenQuad(batch, distortRT.getRenderTarget(), distancesRT.getTexture2D(), nullptr, distortPS, 255);

	// Apply horizontal reduction to the distorted texture
	ApplyReduction(batch, &distortRT, &shadowMapRT);

	context->setRenderTarget(nullptr);

	// Set the shader's Texture
	context->setPSTexture2Ds(&shadowMapRT.getTexture2D(), 1, 1);

	// Draw the Shadows using ShadowMap and Distorted Textures into the shadowsRT Render Texture
	renderFullscreenQuad(batch, shadowsRT.getRenderTarget(), distortRT.getTexture2D(), nullptr, shadowPS, 255);

	// Apply Blur to the shadows for better looking
	TextureSize size;
	context->getTexture2DSize(shadowsRT.getTexture2D(), &size);

	setBlurParameters(1.0f / (float)size.width, 0.0f);
	renderFullscreenQuad(batch, processedShadowRT.getRenderTarget(), shadowsRT.getTexture2D(), nullptr, gaussianBlurPS, 255);

	// Draw the processed shadows into the scene Render Target 
	renderFullscreenQuad(batch, sceneRenderTarget, processedShadowRT.getTexture2D(), nullptr, nullptr, 255);


	hTexture2D nullTex;
	context->setPSTexture2Ds(&nullTex, 1, 1);

	context->setDepthStencilState(context->DSSDefault);
}

void ShadowMap::renderFullscreenQuad(SpriteBatch& batch, hRenderTarget destination, hTexture2D source, hVertexShader vertexShader, hPixelShader pixelShader, float alpha) const
{
	TextureSize surfSize;
	context->getRenderTargetSize(destination, 0, &surfSize);

	TextureSize texSize;
	context->getTexture2DSize(source, &texSize);

	Sprite sprite;
	sprite.setPosition(0, 0);
	sprite.setSize((float)surfSize.width, (float)surfSize.height);
	sprite.setSrcRect(FloatRect(0, 0, (float)texSize.width, (float)texSize.height));
	sprite.setTexture(source);
	sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha));

	batch.begin(destination, SpriteSortMode_Deferred, nullptr, vertexShader, pixelShader);
	batch.draw(sprite);
	batch.end();
}

void ShadowMap::setBlurParameters(float dx, float dy) const
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

float ShadowMap::computeGaussian(float n) const
{
	float theta = 4.0f;

	// sqrt(2 * pi) = 2.50662827463
	return (float)((1.0 / (2.50662827463 * theta)) * std::exp(-(n * n) / (2.0 * theta * theta)));
}
