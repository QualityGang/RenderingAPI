#pragma once

#include <PixelFormat.h>
#include <NonCopyable.h>

#include "D3D11GraphicsContext.h"
#include "D3D11DLLExport.h"

class D3D11RenderWindow : public NonCopyable
{
public:
	DLL_D3D11_API D3D11RenderWindow(
		const D3D11GraphicsContext *context,
		HWND						hwnd,
		uint32_t					width,
		uint32_t					height,
		uint32_t					bufferCount,
		PixelFormat					bufferFormat,
		uint32_t					numSamples,
		bool						vsync
	);
	DLL_D3D11_API ~D3D11RenderWindow();

	DLL_D3D11_API hTexture2D getBackBuffer() const;
	DLL_D3D11_API void swapBuffers() const;
	DLL_D3D11_API void resize(uint32_t width, uint32_t height);

	void setVSyncEnabled(bool vsync) { this->vsync = vsync; }
	bool isVSyncEnabled() const { return vsync; }

	hRenderTarget getRenderTarget() const { return renderTarget; }
private:
	void initRenderTarget(uint32_t width, uint32_t height);

	const D3D11GraphicsContext *context;
	IDXGISwapChain *swapChain;
	hRenderTarget renderTarget;
	bool vsync;
};

