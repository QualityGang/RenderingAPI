#include "GraphicsContext.h"

extern "C" DLL_REN_API void GraphicsContext_GetTexture2DSize(GraphicsContext *context, void *texture, TextureSize *texSize)
{
	context->getTexture2DSize(hTexture2D(texture), texSize);
}

extern "C" DLL_REN_API void GraphicsContext_EnableAdditiveBlending(GraphicsContext *context)
{
	context->setBlendState(context->BSAdditive);
}

extern "C" DLL_REN_API void GraphicsContext_DisableAdditiveBlending(GraphicsContext *context)
{
	context->setBlendState(context->BSAlphaBlend);
}