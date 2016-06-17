#include "D3D11RenderWindow.h"

#include <Throw.h>

#include "D3D11Resource.h"
#include "D3D11Macros.h"
#include "D3D11Maps.h"

D3D11RenderWindow::D3D11RenderWindow(
	const D3D11GraphicsContext *context,
	HWND						hwnd,
	uint32_t					width,
	uint32_t					height,
	uint32_t					bufferCount,
	PixelFormat					bufferFormat,
	uint32_t					numSamples,
	bool						vsync
) : context(context), vsync(vsync)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = bufferCount;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = Map_PixelFormat[bufferFormat];
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = numSamples;
	desc.SampleDesc.Quality = 0;
	desc.Flags = 0;
	desc.Windowed = TRUE;

	IDXGIFactory *dxgiFactory = context->getDefaultDXGIFactory();
	HRESULT hr = dxgiFactory->CreateSwapChain(context->getNativeDevice(), &desc, &swapChain);
	SafeRelease(dxgiFactory);

	if (FAILED(hr))
		THROW("Failed to create swap chain");

	initRenderTarget(width, height);
}

D3D11RenderWindow::~D3D11RenderWindow()
{
	SafeRelease(swapChain);

	context->releaseRenderTarget(renderTarget);
}

hTexture2D D3D11RenderWindow::getBackBuffer() const
{
	ID3D11Texture2D *backBuffer;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	if (FAILED(hr))
		THROW("Failed to obtian back buffer");

	D3D11Resource *res = new D3D11Resource;
	res->resource = backBuffer;
	res->srv = nullptr;

	return hTexture2D(res);
}

void D3D11RenderWindow::swapBuffers() const
{
	HRESULT hr = swapChain->Present((vsync ? 1 : 0), 0);
	
	if (FAILED(hr))
		THROW("Failed to swap buffers");
}

void D3D11RenderWindow::resize(uint32_t width, uint32_t height)
{
	// unbind and release rt
	context->setRenderTarget(nullptr);
	context->releaseRenderTarget(renderTarget);

	// resize sc buffers and recreate rt
	DXGI_SWAP_CHAIN_DESC scDesc;
	HRESULT hr = swapChain->GetDesc(&scDesc);

	if (FAILED(hr))
		THROW("Failed to get swap chain desc");

	hr = swapChain->ResizeBuffers(scDesc.BufferCount, width, height, scDesc.BufferDesc.Format, scDesc.Flags);

	if (FAILED(hr))
		THROW("Failed to resize swap chain buffer");

	initRenderTarget(width, height);
}

void D3D11RenderWindow::initRenderTarget(uint32_t width, uint32_t height)
{
	hTexture2D backBuffer = getBackBuffer();
	hTexture2D depthStencilBuffer = context->createTexture2D(
		width, height, PixelFormat_D24S8,
		1, 1, TextureFlag_DepthStencil, AccessFlag_Default, nullptr, 0
	);

	RenderTarget rtDesc;
	rtDesc.numColorTextures    = 1;
	rtDesc.colorTextures[0]    = backBuffer;
	rtDesc.depthStencilTexture = depthStencilBuffer;

	renderTarget = context->createRenderTarget(rtDesc);

	context->releaseTexture2D(backBuffer);
	context->releaseTexture2D(depthStencilBuffer);
}
