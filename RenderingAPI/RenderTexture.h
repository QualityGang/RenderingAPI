#pragma once

#include "GraphicsContext.h"
#include "Bitmap.h"
#include "NonCopyable.h"

class RenderTexture : public NonCopyable
{
public:
	DLL_REN_API explicit RenderTexture(GraphicsContext *context);
	DLL_REN_API RenderTexture(GraphicsContext *context, uint32_t width, uint32_t height, PixelFormat format = PixelFormat_RGBA8);
	DLL_REN_API RenderTexture(GraphicsContext *context, const Bitmap &bmp);
	DLL_REN_API ~RenderTexture();

	DLL_REN_API void create(uint32_t width, uint32_t height, PixelFormat format = PixelFormat_RGBA8);
	DLL_REN_API void create(const Bitmap &bmp);

	hRenderTarget getRenderTarget() const { return renderTarget; }
	hTexture2D getTexture2D() const { return texture; }
private:
	void initRenderTarget(hTexture2D texture, uint32_t width, uint32_t height);

	GraphicsContext *context;
	hRenderTarget renderTarget;
	hTexture2D texture;
};

