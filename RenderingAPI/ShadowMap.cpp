#include "ShadowMap.h"

ShadowMap::ShadowMap(GraphicsContext* context, ShadowMapSize size) : context(context), DistancesRT(context)
{
	//ComputeDistancesPS = context->createPixelShader(g_ShadowEffectPS, sizeof(g_ShadowEffectPS));
	//ComputeDistancesCB = context->createBuffer(BufferType_ConstantBuffer, 16, AccessFlag_Write, nullptr);
}

ShadowMap::~ShadowMap()
{
	context->releasePixelShader(ComputeDistancesPS);
	context->releaseBuffer(ComputeDistancesCB);
}

void ShadowMap::setRenderTarget(hRenderTarget renderTarget)
{
	shadowRenderTarget = renderTarget;

	if (!shadowRenderTarget)
		return;

	TextureSize rtSize;
	context->getRenderTargetSize(renderTarget, 0, &rtSize);

	if (size.width != rtSize.width || size.height != rtSize.height)
	{
		size = rtSize;

		DistancesRT.create(size.width, size.height,PixelFormat_RG16F);
	}
}

void ShadowMap::draw(SpriteBatch &batch)
{
}
