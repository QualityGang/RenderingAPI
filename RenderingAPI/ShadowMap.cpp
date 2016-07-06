#include "ShadowMap.h"
#include "ComputeDistancesPS.h"
#include "DistortPS.h"

ShadowMap::ShadowMap(GraphicsContext* context, ShadowMapSize size) :
	context(context), distancesRT(context), distortRT(context), sceneRenderTexture(context)
{
	shadowMapSize = 2 << size;
	ComputeDistancesPS = context->createPixelShader(g_ComputeDistancesPS, sizeof(g_ComputeDistancesPS));
	ComputeDistancesCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);

	DistortPS = context->createPixelShader(g_DistortPS, sizeof(g_DistortPS));
	DistortCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);
}

ShadowMap::~ShadowMap()
{
	context->releasePixelShader(ComputeDistancesPS);
	context->releasePixelShader(DistortPS);
	context->releaseBuffer(DistortCB);
	context->releaseBuffer(ComputeDistancesCB);
}

#include "Bitmap.h";

void ShadowMap::setRenderTarget(hRenderTarget renderTarget)
{
	sceneRenderTarget = renderTarget;

	distancesRT.create(shadowMapSize, shadowMapSize, PixelFormat_RG16F);
	distortRT.create(shadowMapSize, shadowMapSize, PixelFormat_RG16F);

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

void ShadowMap::draw(SpriteBatch &batch, Color backgroundColor)
{
	if (!sceneRenderTarget)
		return;

	hTexture2D surface = context->getTexture2D(sceneRenderTarget, 0);
	context->copyTexture2D(surface, sceneRenderTexture.getTexture2D());
	context->releaseTexture2D(surface);
	context->setDepthStencilState(context->DSSNone);
	MapData mapData;
	context->mapBuffer(ComputeDistancesCB, MapType_Write, &mapData);
	float *bgColor = (float*)mapData.mem;
	bgColor[0] = (float)backgroundColor.r / 255.0f;
	bgColor[1] = (float)backgroundColor.g / 255.0f;
	bgColor[2] = (float)backgroundColor.b / 255.0f;
	bgColor[3] = (float)backgroundColor.a / 255.0f;
	context->unmapBuffer(ComputeDistancesCB);
	context->setPSConstantBuffers(&ComputeDistancesCB, 0, 1);

	renderFullscreenQuad(batch, distancesRT.getRenderTarget(), sceneRenderTexture.getTexture2D(), ComputeDistancesPS, 255);
	//renderFullscreenQuad(batch, distortRT.getRenderTarget(), distancesRT.getTexture2D(), DistortPS, 255);
	renderFullscreenQuad(batch, sceneRenderTarget, distancesRT.getTexture2D(), nullptr, 255);
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
