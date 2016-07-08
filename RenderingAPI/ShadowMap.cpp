#include "ShadowMap.h"
#include "ComputeDistancesPS.h"
#include "DistortPS.h"
#include "HorizontalReductionPS.h"
#include "DrawShadowPS.h"

ShadowMap::ShadowMap(GraphicsContext* context, ShadowMapSize size) :
	context(context), distancesRT(context), distortRT(context), shadowRT(context), sceneRenderTexture(context)
{
	shadowMapSize = 2 << size;
	reductionChainCount = size;

	computeDistancesPS = context->createPixelShader(g_ComputeDistancesPS, sizeof(g_ComputeDistancesPS));
	computeDistancesCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);

	distortPS = context->createPixelShader(g_DistortPS, sizeof(g_DistortPS));
	//distortCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);

	hReductionPS = context->createPixelShader(g_HorizontalReductionPS, sizeof(g_HorizontalReductionPS));
	hReductionCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);

	for (int i = 0; i < reductionChainCount; ++i)
	{
		reductionRT.push_back(new RenderTexture(context));
	}

	shadowPS = context->createPixelShader(g_DrawShadowPS, sizeof(g_DrawShadowPS));
	//shadowCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);
}

ShadowMap::~ShadowMap()
{
	context->releasePixelShader(computeDistancesPS);
	context->releasePixelShader(distortPS);
	context->releasePixelShader(hReductionPS);
	context->releaseBuffer(distortCB);
	context->releaseBuffer(computeDistancesCB);
	context->releaseBuffer(hReductionCB);

	for (auto rt : reductionRT)
	{
		delete rt;
	}
}

void ShadowMap::setRenderTarget(hRenderTarget renderTarget)
{
	sceneRenderTarget = renderTarget;

	distancesRT.create(shadowMapSize, shadowMapSize, PixelFormat_RG16F);
	distortRT.create(shadowMapSize, shadowMapSize, PixelFormat_RG16F);

	if (!sceneRenderTarget)
		return;

	for (int i = 0; i < reductionRT.size(); ++i)
	{
		reductionRT[i]->create(2 << i, shadowMapSize, PixelFormat_RG16F);
	}

	shadowRT.create(shadowMapSize, shadowMapSize);

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

		renderFullscreenQuad(batch, reductionRT[step]->getRenderTarget(), source->getTexture2D(), nullptr, 255);

		source = reductionRT[step];

		step--;
	}

	renderFullscreenQuad(batch, destination->getRenderTarget(), reductionRT[step + 1]->getTexture2D(), hReductionPS, 255);
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

	renderFullscreenQuad(batch, distancesRT.getRenderTarget(), sceneRenderTexture.getTexture2D(), computeDistancesPS, 255);
	renderFullscreenQuad(batch, distortRT.getRenderTarget(), distancesRT.getTexture2D(), distortPS, 255);

	ApplyReduction(batch, &distortRT, &shadowRT);

	renderFullscreenQuad(batch, sceneRenderTarget, distortRT.getTexture2D(), nullptr, 255);
}

void ShadowMap::renderFullscreenQuad(SpriteBatch& batch, hRenderTarget renderTarget, hTexture2D texture, hPixelShader pixelShader, float alpha) const
{
	TextureSize surfSize;
	context->getRenderTargetSize(renderTarget, 0, &surfSize);

	TextureSize texSize;
	context->getTexture2DSize(texture, &texSize);

	Sprite sprite;
	sprite.setPosition(0, 0);
	sprite.setSize((float)surfSize.width, (float)surfSize.height);
	sprite.setSrcRect(FloatRect(0, 0, (float)texSize.width, (float)texSize.height));
	sprite.setTexture(texture);
	sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha));

	batch.begin(renderTarget, SpriteSortMode_Deferred, nullptr, nullptr, pixelShader);
	batch.draw(sprite);
	batch.end();
}
