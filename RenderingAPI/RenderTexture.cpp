#include "RenderTexture.h"

#include "Throw.h"

RenderTexture::RenderTexture(GraphicsContext *context)
	: context(context)
{
}

RenderTexture::RenderTexture(GraphicsContext *context, uint32_t width, uint32_t height)
	: context(context)
{
	create(width, height);
}

RenderTexture::RenderTexture(const Bitmap &bmp)
{
	THROW("Not implemented");
}

RenderTexture::~RenderTexture()
{
	context->releaseRenderTarget(renderTarget);
	context->releaseTexture2D(texture);
}

void RenderTexture::create(uint32_t width, uint32_t height)
{
	texture = context->createTexture2D(width, height, PixelFormat_RGBA8, 1, 1,
		TextureFlag_RenderTarget | TextureFlag_ShaderResource, AccessFlag_Default, nullptr, 0);

	initRenderTarget(texture, width, height);
}

void RenderTexture::initRenderTarget(hTexture2D texture, uint32_t width, uint32_t height)
{
	hTexture2D depthStencilBuffer = context->createTexture2D(
		width, height, PixelFormat_D24S8,
		1, 1, TextureFlag_DepthStencil, AccessFlag_Default, nullptr, 0
	);

	RenderTarget rtDesc;
	rtDesc.numColorTextures = 1;
	rtDesc.colorTextures[0] = texture;
	rtDesc.depthStencilTexture = depthStencilBuffer;

	renderTarget = context->createRenderTarget(rtDesc);
	context->releaseTexture2D(depthStencilBuffer);
}
