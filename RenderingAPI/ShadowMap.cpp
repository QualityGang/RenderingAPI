#include "ShadowMap.h"
#include "ComputeDistancesPS.h"

ShadowMap::ShadowMap(GraphicsContext* context, ShadowMapSize size) : context(context), distancesRT(context), sceneRenderTexture(context)
{
	shadowMapSize = 2 << size;
	ComputeDistancesPS = context->createPixelShader(g_ComputeDistancesPS, sizeof(g_ComputeDistancesPS));
	ComputeDistancesCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);
}

ShadowMap::~ShadowMap()
{
	context->releasePixelShader(ComputeDistancesPS);
	context->releaseBuffer(ComputeDistancesCB);
}

#include "Bitmap.h";

void ShadowMap::setRenderTarget(hRenderTarget renderTarget)
{
	sceneRenderTarget = renderTarget;

	distancesRT.create(shadowMapSize, shadowMapSize, PixelFormat_RG16F);

	if (!sceneRenderTarget)
		return;

	// Look up the resolution of our source render target.
	TextureSize rtSize;
	context->getRenderTargetSize(renderTarget, 0, &rtSize);

	if (size.width != rtSize.width || size.height != rtSize.height)
	{
		size = rtSize;

		sceneRenderTexture.create(size.width, size.height);
	}
}

void ShadowMap::draw(SpriteBatch &batch)
{
	if (!sceneRenderTarget)
		return;

	hTexture2D surface = context->getTexture2D(sceneRenderTarget, 0);
	context->copyTexture2D(surface, sceneRenderTexture.getTexture2D());
	context->releaseTexture2D(surface);

	context->setDepthStencilState(context->DSSNone);

	renderFullscreenQuad(batch, distancesRT.getRenderTarget(), sceneRenderTexture.getTexture2D(), ComputeDistancesPS);

	context->setDepthStencilState(context->DSSDefault);
}

void ShadowMap::renderFullscreenQuad(SpriteBatch& batch, hRenderTarget renderTarget, hTexture2D texture, hPixelShader pixelShader) const
{
	TextureSize surfSize;
	context->getRenderTargetSize(renderTarget, 0, &surfSize);

	TextureSize texSize;
	context->getTexture2DSize(texture, &texSize);

	Sprite sprite;
	sprite.setPosition(400, 300);
	sprite.setSize((float)200, (float)150);
	sprite.setSrcRect(FloatRect(0, 0, (float)texSize.width, (float)texSize.height));
	sprite.setTexture(texture);

	batch.begin(sceneRenderTarget, SpriteSortMode_Deferred, nullptr, nullptr, ComputeDistancesPS);
	batch.draw(sprite);
	batch.end();
}
