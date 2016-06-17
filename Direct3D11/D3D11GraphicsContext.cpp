#include "D3D11GraphicsContext.h"

#include <Throw.h>

#include "D3D11RenderWindow.h"
#include "D3D11Resource.h"
#include "D3D11RenderTarget.h"
#include "D3D11Macros.h"
#include "D3D11Maps.h"

D3D11GraphicsContext::D3D11GraphicsContext()
{
	UINT flags = 0;
#if _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
		&featureLevel, 1, D3D11_SDK_VERSION, &device, nullptr, &immContext);

	if (FAILED(hr))
		THROW("Failed to create device");
}

D3D11GraphicsContext::~D3D11GraphicsContext()
{
//#if _DEBUG
//	ID3D11Debug *d3d11Debug;
//	HRESULT hr = device->QueryInterface(&d3d11Debug);
//
//	// throwing in dtors is a broken concept
//	if (SUCCEEDED(hr))
//	{
//		d3d11Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//		SafeRelease(d3d11Debug);
//	}
//#endif

	SafeRelease(device);
	SafeRelease(immContext);
}

uint32_t D3D11GraphicsContext::getPixelFormatStride(PixelFormat format) const
{
	switch (format)
	{
		case PixelFormat_R8:      return 1;
		case PixelFormat_R16:     return 2;
		case PixelFormat_R16F:    return 2;
		case PixelFormat_R32I:    return 4;
		case PixelFormat_R32U:    return 4;
		case PixelFormat_R32F:    return 4;
		case PixelFormat_RG8:     return 2;
		case PixelFormat_RG16:    return 4;
		case PixelFormat_RG16F:   return 4;
		case PixelFormat_RG32I:   return 8;
		case PixelFormat_RG32U:   return 8;
		case PixelFormat_RG32F:   return 8;
		case PixelFormat_RGB32I:  return 12;
		case PixelFormat_RGB32U:  return 12;
		case PixelFormat_RGB32F:  return 12;
		case PixelFormat_RGBA8:   return 4;
		case PixelFormat_RGBA16:  return 8;
		case PixelFormat_RGBA16F: return 8;
		case PixelFormat_RGBA32I: return 16;
		case PixelFormat_RGBA32U: return 16;
		case PixelFormat_RGBA32F: return 16;
	}

	THROW("Unsupported pixel format");
}

IDXGIDevice* D3D11GraphicsContext::getDefaultDXGIDevice() const
{
	IDXGIDevice *dxgiDevice;
	HRESULT hr = device->QueryInterface(&dxgiDevice);

	if (FAILED(hr))
		THROW("Failed to obtain default dxgi device");

	return dxgiDevice;
}

IDXGIAdapter* D3D11GraphicsContext::getDefaultDXGIAdapter() const
{
	IDXGIDevice *dxgiDevice = getDefaultDXGIDevice();

	IDXGIAdapter *dxgiAdapter;
	HRESULT hr = dxgiDevice->GetAdapter(&dxgiAdapter);
	SafeRelease(dxgiDevice);

	if (FAILED(hr))
		THROW("Failed to obtain default dxgi adapter");

	return dxgiAdapter;
}

IDXGIFactory* D3D11GraphicsContext::getDefaultDXGIFactory() const
{
	IDXGIAdapter *dxgiAdapter = getDefaultDXGIAdapter();

	IDXGIFactory *dxgiFactory;
	HRESULT hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	SafeRelease(dxgiAdapter);

	if (FAILED(hr))
		THROW("Failed to obtain default dxgi adapter");

	return dxgiFactory;
}

hVertexShader D3D11GraphicsContext::createVertexShader(const void *bytecode, size_t bytecodeSize) const
{
	ID3D11VertexShader *d3d11Shader;
	HRESULT hr = device->CreateVertexShader(bytecode, bytecodeSize, nullptr, &d3d11Shader);

	if (FAILED(hr))
		THROW("Failed to create vertex shader");

	return hVertexShader(d3d11Shader);
}

void D3D11GraphicsContext::releaseVertexShader(hVertexShader shader) const
{
	ID3D11VertexShader *d3d11Shader = shader.as<ID3D11VertexShader>();
	SafeRelease(d3d11Shader);
}

void D3D11GraphicsContext::setVertexShader(hVertexShader shader) const
{
	immContext->VSSetShader(shader.as<ID3D11VertexShader>(), nullptr, 0);
}

hPixelShader D3D11GraphicsContext::createPixelShader(const void *bytecode, size_t bytecodeSize) const
{
	ID3D11PixelShader *d3d11Shader;
	HRESULT hr = device->CreatePixelShader(bytecode, bytecodeSize, nullptr, &d3d11Shader);

	if (FAILED(hr))
		THROW("Failed to create pixel shader");

	return hPixelShader(d3d11Shader);
}

void D3D11GraphicsContext::releasePixelShader(hPixelShader shader) const
{
	ID3D11PixelShader *d3d11Shader = shader.as<ID3D11PixelShader>();
	SafeRelease(d3d11Shader);
}

void D3D11GraphicsContext::setPixelShader(hPixelShader shader) const
{
	immContext->PSSetShader(shader.as<ID3D11PixelShader>(), nullptr, 0);
}

hVertexFormat D3D11GraphicsContext::createVertexFormat(
	VertexElement *elements,
	uint32_t	  numElements,
	const void	  *shaderBytecode,
	size_t		  shaderBytecodeSize
) const
{
#if _DEBUG
	if (!elements || numElements == 0)
		THROW("Invalid arguments");
#endif

	D3D11_INPUT_ELEMENT_DESC *inputData = new D3D11_INPUT_ELEMENT_DESC[numElements];

	for (uint32_t i = 0; i < numElements; i++)
	{
		inputData[i].SemanticName = elements[i].semanticName;
		inputData[i].SemanticIndex = elements[i].semanticIndex;
		inputData[i].Format = Map_PixelFormat[elements[i].format];
		inputData[i].InputSlot = elements[i].slot;
		inputData[i].AlignedByteOffset = elements[i].offset;
		inputData[i].InputSlotClass = Map_InputDataType[elements[i].inputType];
		inputData[i].InstanceDataStepRate = elements[i].instanceDataStepRate;
	}

	ID3D11InputLayout *layout;
	HRESULT hr = device->CreateInputLayout(inputData, numElements, shaderBytecode, shaderBytecodeSize, &layout);

	delete[] inputData;

	if (FAILED(hr))
		THROW("Failed to create vertex format");

	return hVertexFormat(layout);
}

void D3D11GraphicsContext::releaseVertexFormat(hVertexFormat format) const
{
	ID3D11InputLayout *layout = format.as<ID3D11InputLayout>();
	SafeRelease(layout);
}

void D3D11GraphicsContext::setVertexFormat(hVertexFormat format) const
{
	immContext->IASetInputLayout(format.as<ID3D11InputLayout>());
}

hBuffer D3D11GraphicsContext::createBuffer(BufferType type, uint32_t size, uint32_t cpuAccess, const void *mem) const
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = size;
	desc.BindFlags = Map_BufferType[type];
	
	SetAccessFlags(&desc, cpuAccess);

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = mem;

	ID3D11Buffer *buffer;
	HRESULT hr = device->CreateBuffer(&desc, (mem ? &data : nullptr), &buffer);

	if (FAILED(hr))
		THROW("Failed to create buffer");

	return hBuffer(buffer);
}

void D3D11GraphicsContext::releaseBuffer(hBuffer buffer) const
{
	ID3D11Buffer *d3d11Buffer = buffer.as<ID3D11Buffer>();
	SafeRelease(d3d11Buffer);
}

void D3D11GraphicsContext::mapBuffer(hBuffer buffer, MapType type, MapData *data) const
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HRESULT hr = immContext->Map(buffer.as<ID3D11Buffer>(), 0, Map_MapType[type], 0, &mappedData);

	if (FAILED(hr))
		THROW("Failed to map buffer");

	data->mem         = mappedData.pData;
	data->rowPitch    = mappedData.RowPitch;
	data->depthPitch  = mappedData.DepthPitch;
}

void D3D11GraphicsContext::unmapBuffer(hBuffer buffer) const
{
	immContext->Unmap(buffer.as<ID3D11Buffer>(), 0);
}

hRasterizerState D3D11GraphicsContext::createRasterizerState(const RasterizerState &state) const
{
	D3D11_RASTERIZER_DESC desc;
	desc.FillMode = Map_FillMode[state.fillMode];
	desc.CullMode = Map_CullMode[state.cullMode];
	desc.FrontCounterClockwise = Map_VertexOrder[state.vertexOrder];
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = TRUE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;

	ID3D11RasterizerState *d3d11State;

	HRESULT hr = device->CreateRasterizerState(&desc, &d3d11State);

	if (FAILED(hr))
		THROW("Failed to create rasterizer state");

	return hRasterizerState(d3d11State);
}

void D3D11GraphicsContext::releaseRasterizerState(hRasterizerState state) const
{
	ID3D11RasterizerState *d3d11State = state.as<ID3D11RasterizerState>();
	SafeRelease(d3d11State);
}

void D3D11GraphicsContext::setRasterizerState(hRasterizerState state) const
{
	immContext->RSSetState(state.as<ID3D11RasterizerState>());
}

hBlendState D3D11GraphicsContext::createBlendState(const BlendState &state) const
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable  = state.alphaToCoverageEnabled;
	desc.IndependentBlendEnable = state.separateBlendEnabled;
	
	uint32_t numIterations = state.separateBlendEnabled ? 8 : 1;
	
	for (uint32_t i = 0; i < numIterations; i++)
	{
		desc.RenderTarget[i].BlendEnable           = state.blendDesc[i].blendEnabled;
	    desc.RenderTarget[i].RenderTargetWriteMask = /*Map_ColorWriteMask[*/state.blendDesc[i].writeMask/*]*/;
	    desc.RenderTarget[i].SrcBlend              = Map_BlendFactor[state.blendDesc[i].srcBlend];
	    desc.RenderTarget[i].DestBlend             = Map_BlendFactor[state.blendDesc[i].dstBlend];
	    desc.RenderTarget[i].BlendOp               = Map_BlendOp[state.blendDesc[i].blendOp];
	    desc.RenderTarget[i].SrcBlendAlpha         = Map_BlendFactor[state.blendDesc[i].srcBlendAlpha];
	    desc.RenderTarget[i].DestBlendAlpha        = Map_BlendFactor[state.blendDesc[i].dstBlendAlpha];
	    desc.RenderTarget[i].BlendOpAlpha          = Map_BlendOp[state.blendDesc[i].blendOpAlpha];
	}

	ID3D11BlendState *d3d11State;
	HRESULT hr = device->CreateBlendState(&desc, &d3d11State);

	if (FAILED(hr))
		THROW("Failed to create blend state");

	return hBlendState(d3d11State);
}

void D3D11GraphicsContext::releaseBlendState(hBlendState state) const
{
	ID3D11BlendState *d3d11State = state.as<ID3D11BlendState>();
	SafeRelease(d3d11State);
}

void D3D11GraphicsContext::setBlendState(hBlendState state) const
{
	immContext->OMSetBlendState(state.as<ID3D11BlendState>(), nullptr, 0xFFFFFFFF);
}

hDepthStencilState D3D11GraphicsContext::createDepthStencilState(const DepthStencilState &state) const
{
	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable                  = state.depthEnabled;
	desc.DepthWriteMask               = Map_DepthWriteMask[state.writeMask];
	desc.DepthFunc                    = Map_ComparisonFunc[state.depthFunc];
	desc.StencilEnable                = state.stencilEnabled;
	desc.StencilReadMask              = state.stencilReadMask;
	desc.StencilWriteMask             = state.stencilWriteMask;
	desc.FrontFace.StencilFunc        = Map_ComparisonFunc[state.frontFaceStencilDesc.stencilFunc];
	desc.FrontFace.StencilFailOp      = Map_StencilOp[state.frontFaceStencilDesc.failOp];
	desc.FrontFace.StencilDepthFailOp = Map_StencilOp[state.frontFaceStencilDesc.depthFailOp];
	desc.FrontFace.StencilPassOp      = Map_StencilOp[state.frontFaceStencilDesc.passOp];
	desc.BackFace.StencilFunc         = Map_ComparisonFunc[state.backFaceStencilDesc.stencilFunc];
	desc.BackFace.StencilFailOp       = Map_StencilOp[state.backFaceStencilDesc.failOp];
	desc.BackFace.StencilDepthFailOp  = Map_StencilOp[state.backFaceStencilDesc.depthFailOp];
	desc.BackFace.StencilPassOp       = Map_StencilOp[state.backFaceStencilDesc.passOp];

	ID3D11DepthStencilState *d3d11State;
	HRESULT hr = device->CreateDepthStencilState(&desc, &d3d11State);

	if (FAILED(hr))
		THROW("Failed to create depth stencil state");

	return hDepthStencilState(d3d11State);
}

void D3D11GraphicsContext::releaseDepthStencilState(hDepthStencilState state) const
{
	ID3D11DepthStencilState *d3d11State = state.as<ID3D11DepthStencilState>();
	SafeRelease(d3d11State);
}

void D3D11GraphicsContext::setDepthStencilState(hDepthStencilState state) const
{
	immContext->OMSetDepthStencilState(state.as<ID3D11DepthStencilState>(), 1);
}

hSamplerState D3D11GraphicsContext::createSamplerState(const SamplerState &state) const
{
	D3D11_SAMPLER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Filter         = Map_TextureFilter[static_cast<size_t>(state.filter)];
    desc.AddressU       = Map_AddressMode[static_cast<size_t>(state.addressU)];
    desc.AddressV       = Map_AddressMode[static_cast<size_t>(state.addressV)];
    desc.AddressW       = Map_AddressMode[static_cast<size_t>(state.addressW)];
    desc.MipLODBias     = state.lodBias;
    desc.MaxAnisotropy  = state.maxAnisotropy;
    desc.ComparisonFunc = Map_ComparisonFunc[static_cast<uint32_t>(state.comparisonFunc)];
    desc.BorderColor[0] = state.borderColor.r / 255.0f;
    desc.BorderColor[1] = state.borderColor.g / 255.0f;
    desc.BorderColor[2] = state.borderColor.b / 255.0f;
    desc.BorderColor[3] = state.borderColor.a / 255.0f;
    desc.MinLOD         = state.minLod;
    desc.MaxLOD         = state.maxLod;

	ID3D11SamplerState *d3d11State;
	HRESULT hr = device->CreateSamplerState(&desc, &d3d11State);

	if (FAILED(hr))
		THROW("Failed to create sampler state");

	return hSamplerState(d3d11State);
}

void D3D11GraphicsContext::releaseSamplerState(hSamplerState state) const
{
	ID3D11SamplerState *d3d11State = state.as<ID3D11SamplerState>();
	SafeRelease(d3d11State);
}

void D3D11GraphicsContext::setPSSamplers(const hSamplerState *states, uint32_t startSlot, uint32_t numSamplers) const
{
	ID3D11SamplerState **samplers = new ID3D11SamplerState*[numSamplers];

	for (uint32_t i = 0; i < numSamplers; i++)
		samplers[i] = states[i].as<ID3D11SamplerState>();

	immContext->PSSetSamplers(startSlot, numSamplers, samplers);

	delete[] samplers;
}

hTexture2D D3D11GraphicsContext::createTexture2D(
	uint32_t	  width,
	uint32_t	  height,
	PixelFormat	  format,
	uint32_t	  numMipmaps,
	uint32_t	  numSamples,
	uint32_t	  texFlags,
	uint32_t	  cpuAccess,
	void	      *mem,
	uint32_t	  rowPitch
) const
{
	D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width              = width;
    desc.Height             = height;
    desc.MipLevels          = numMipmaps;
    desc.ArraySize          = 1;
    desc.Format             = Map_PixelFormat[format];
    desc.MiscFlags          = 0;
    desc.SampleDesc.Count   = numSamples;
    desc.SampleDesc.Quality = 0;

	SetTextureFlags(&desc, texFlags);
	SetAccessFlags(&desc, cpuAccess);

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem     = mem;
	data.SysMemPitch = rowPitch;

    ID3D11Texture2D *texture;
	HRESULT hr = device->CreateTexture2D(&desc, (mem ? &data : nullptr), &texture);

	if (FAILED(hr))
		THROW("Failed to create 2D texture");
	
	ID3D11ShaderResourceView *srv = nullptr;

	if (texFlags & TextureFlag_ShaderResource)
	{
		hr = device->CreateShaderResourceView(texture, nullptr, &srv);

		if (FAILED(hr))
		{
			SafeRelease(texture);
			THROW("Failed to create srv for 2d texture");
		}
	}

	D3D11Resource *res = new D3D11Resource;
	res->resource = texture;
	res->srv      = srv;

	return hTexture2D(res);
}

void D3D11GraphicsContext::releaseTexture2D(hTexture2D texture) const
{
	D3D11Resource *res = texture.as<D3D11Resource>();
	
	if (res)
	{
		SafeRelease(res->resource);
		SafeRelease(res->srv);
		delete res;
	}
}

void D3D11GraphicsContext::getTexture2DSize(hTexture2D texture, TextureSize *texSize) const
{
	D3D11Resource   *res		  = texture.as<D3D11Resource>();
	ID3D11Texture2D *d3d11Texture = (ID3D11Texture2D*)res->resource;

	D3D11_TEXTURE2D_DESC desc;
	d3d11Texture->GetDesc(&desc);

	texSize->width  = desc.Width;
	texSize->height = desc.Height;
	texSize->depth  = 0;
}

void D3D11GraphicsContext::mapTexture2D(hTexture2D texture, MapType type, MapData *data) const
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HRESULT hr = immContext->Map(texture.as<D3D11Resource>()->resource, 0, Map_MapType[type], 0, &mappedData);

	if (FAILED(hr))
		THROW("Failed to map texture");

	data->mem        = mappedData.pData;
	data->rowPitch   = mappedData.RowPitch;
	data->depthPitch = mappedData.DepthPitch;
}

void D3D11GraphicsContext::unmapTexture2D(hTexture2D texture) const
{
	immContext->Unmap(texture.as<D3D11Resource>()->resource, 0);
}

hRenderTarget D3D11GraphicsContext::createRenderTarget(const RenderTarget &renderTarget) const
{
	D3D11RenderTarget *rt = new D3D11RenderTarget;
	ZeroMemory(rt, sizeof(D3D11RenderTarget));

	rt->numRenderTargets = renderTarget.numColorTextures;

	for (uint32_t i = 0; i < renderTarget.numColorTextures; i++)
	{
		D3D11Resource *rtRes = renderTarget.colorTextures[i].as<D3D11Resource>();

		ID3D11RenderTargetView *renderTargetView;
		HRESULT hr = device->CreateRenderTargetView(rtRes->resource, nullptr, &renderTargetView);
		
		if (FAILED(hr))
		{
			releaseRenderTarget(hRenderTarget(rt));
			THROW("Failed to create rtv");
		}

		rt->renderTargetViews[i] = renderTargetView;
	}

	D3D11Resource *dsRes = renderTarget.depthStencilTexture.as<D3D11Resource>();
	
	if (dsRes)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(dsDesc));

        ID3D11DepthStencilView *depthStencilView;

		HRESULT hr = device->CreateDepthStencilView(dsRes->resource, nullptr, &depthStencilView);
		
		if (FAILED(hr))
		{
			releaseRenderTarget(hRenderTarget(rt));
			THROW("Failed to create dsv");
        }

        rt->depthStencilView = depthStencilView;
    }

	return hRenderTarget(rt);
}

void D3D11GraphicsContext::releaseRenderTarget(hRenderTarget renderTarget) const
{
	D3D11RenderTarget *rt = renderTarget.as<D3D11RenderTarget>();

	if (rt)
	{
		for (uint32_t i = 0; i < rt->numRenderTargets; i++)
			SafeRelease(rt->renderTargetViews[i]);

		SafeRelease(rt->depthStencilView);

		delete rt;
	}
}

hTexture2D D3D11GraphicsContext::getTexture2D(hRenderTarget renderTarget, uint32_t slot) const
{
	D3D11RenderTarget *rt = renderTarget.as<D3D11RenderTarget>();

	ID3D11Resource *d3d11Resource;
	rt->renderTargetViews[slot]->GetResource(&d3d11Resource);

	ID3D11Texture2D *d3d11Texture2D;
	HRESULT hr = d3d11Resource->QueryInterface(&d3d11Texture2D);

	if (FAILED(hr))
		THROW("Resource is not a 2D texture");

	SafeRelease(d3d11Texture2D);

	D3D11Resource *res = new D3D11Resource;
	res->resource = d3d11Resource;
	res->srv      = nullptr;

	return hTexture2D(res);
}

hRenderWindow D3D11GraphicsContext::createRenderWindow(
	HWND		hwnd,
	uint32_t	width, 
	uint32_t	height,
	uint32_t	bufferCount,
	PixelFormat bufferFormat,
	uint32_t    numSamples,
	bool        vsync
) const
{
	D3D11RenderWindow *renderWindow = new D3D11RenderWindow(this, hwnd, width, height, bufferCount, bufferFormat, numSamples, vsync);
	return hRenderWindow(renderWindow);
}

void D3D11GraphicsContext::releaseRenderWindow(hRenderWindow window) const
{
	D3D11RenderWindow *renderWindow = window.as<D3D11RenderWindow>();
	delete renderWindow;
}

void D3D11GraphicsContext::setVSyncEnabled(hRenderWindow window, bool vsync) const
{
	window.as<D3D11RenderWindow>()->setVSyncEnabled(vsync);
}

bool D3D11GraphicsContext::isVSyncEnabled(hRenderWindow window) const
{
	return window.as<D3D11RenderWindow>()->isVSyncEnabled();
}

hTexture2D D3D11GraphicsContext::getBackBuffer(hRenderWindow window) const
{
	return window.as<D3D11RenderWindow>()->getBackBuffer();
}

void D3D11GraphicsContext::swapBuffers(hRenderWindow window) const
{
	window.as<D3D11RenderWindow>()->swapBuffers();
}

hRenderTarget D3D11GraphicsContext::getRenderTarget(hRenderWindow window) const
{
	return window.as<D3D11RenderWindow>()->getRenderTarget();
}

void D3D11GraphicsContext::resize(hRenderWindow window, uint32_t width, uint32_t height) const
{
	window.as<D3D11RenderWindow>()->resize(width, height);
}

void D3D11GraphicsContext::setViewport(float width, float height, float minDepth, float maxDepth) const
{
	D3D11_VIEWPORT vp;

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = width;
	vp.Height   = height;
	vp.MinDepth = minDepth;
	vp.MaxDepth = maxDepth;

	immContext->RSSetViewports(1, &vp);
}

void D3D11GraphicsContext::setRenderTarget(hRenderTarget renderTarget) const
{
	if (renderTarget)
	{
		D3D11RenderTarget *rt = renderTarget.as<D3D11RenderTarget>();
		immContext->OMSetRenderTargets(rt->numRenderTargets, rt->renderTargetViews, rt->depthStencilView);
	}
	else
	{
		immContext->OMSetRenderTargets(0, nullptr, nullptr);
	}
}

void D3D11GraphicsContext::clearAllRenderTargets(hRenderTarget renderTarget, const Color &color) const
{
	D3D11RenderTarget *rt = renderTarget.as<D3D11RenderTarget>();

	float fcolor[4];
	fcolor[0] = color.r / 255.0f;
	fcolor[1] = color.g / 255.0f;
	fcolor[2] = color.b / 255.0f;
	fcolor[3] = color.a / 255.0f;

	for (size_t i = 0; i < 8; i++)
	{
		if (rt->renderTargetViews[i] != nullptr)
			immContext->ClearRenderTargetView(rt->renderTargetViews[i], fcolor);
	}
}

void D3D11GraphicsContext::clearRenderTarget(hRenderTarget renderTarget, uint32_t slot, const Color &color) const
{
	D3D11RenderTarget *rt = renderTarget.as<D3D11RenderTarget>();

	float fcolor[4];
	fcolor[0] = color.r / 255.0f;
	fcolor[1] = color.g / 255.0f;
	fcolor[2] = color.b / 255.0f;
	fcolor[3] = color.a / 255.0f;

	immContext->ClearRenderTargetView(rt->renderTargetViews[slot], fcolor);
}

void D3D11GraphicsContext::clearDepthStencil(hRenderTarget renderTarget, float depth, uint8_t stencil) const
{
	D3D11RenderTarget *rt = renderTarget.as<D3D11RenderTarget>();
	immContext->ClearDepthStencilView(rt->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void D3D11GraphicsContext::setPrimitiveTopology(PrimitiveTopology topology) const
{
	immContext->IASetPrimitiveTopology(Map_PrimitiveTopology[topology]);
}

void D3D11GraphicsContext::setVertexBuffers(const hBuffer *buffers, uint32_t startSlot, uint32_t numBuffers, const uint32_t *strides, const uint32_t *offsets) const
{
	ID3D11Buffer **d3d11Buffers = new ID3D11Buffer*[numBuffers];

	for (uint32_t i = 0; i < numBuffers; i++)
		d3d11Buffers[i] = buffers[i].as<ID3D11Buffer>();

	immContext->IASetVertexBuffers(startSlot, numBuffers, d3d11Buffers, strides, offsets);

	delete[] d3d11Buffers;
}

void D3D11GraphicsContext::setIndexBuffer(hBuffer buffer, uint32_t offset) const
{
	immContext->IASetIndexBuffer(buffer.as<ID3D11Buffer>(), DXGI_FORMAT_R32_UINT, offset);
}

void D3D11GraphicsContext::setPSConstantBuffers(const hBuffer *buffers, uint32_t startSlot, uint32_t numBuffers) const
{
	ID3D11Buffer **d3d11Buffers = new ID3D11Buffer*[numBuffers];

	for (uint32_t i = 0; i < numBuffers; i++)
		d3d11Buffers[i] = buffers[i].as<ID3D11Buffer>();

	immContext->PSSetConstantBuffers(startSlot, numBuffers, d3d11Buffers);

	delete[] d3d11Buffers;
}

void D3D11GraphicsContext::setPSTexture2Ds(const hTexture2D *textures, uint32_t startSlot, uint32_t numTextures) const
{
	ID3D11ShaderResourceView **SRVs = new ID3D11ShaderResourceView*[numTextures];

	for (uint32_t i = 0; i < numTextures; i++)
	{
		D3D11Resource *res = textures[i].as<D3D11Resource>();

		if (res)
			SRVs[i] = res->srv;
		else
			SRVs[i] = nullptr;
	}

	immContext->PSSetShaderResources(startSlot, numTextures, SRVs);

	delete[] SRVs;
}

void D3D11GraphicsContext::draw(uint32_t count, uint32_t startVertex) const
{
	immContext->Draw(count, startVertex);
}

void D3D11GraphicsContext::drawIndexed(uint32_t count, uint32_t startIndex, uint32_t startVertex) const
{
	immContext->DrawIndexed(count, startIndex, startVertex);
}

void D3D11GraphicsContext::drawInstanced(uint32_t instanceCount, uint32_t count, uint32_t startVertex, uint32_t startInstance) const
{
	immContext->DrawInstanced(count, instanceCount, startVertex, startInstance);
}

void D3D11GraphicsContext::drawIndexedInstanced(uint32_t instanceCount, uint32_t count, uint32_t startIndex, uint32_t startVertex, uint32_t startInstance) const
{
	immContext->DrawIndexedInstanced(count, instanceCount, startIndex, startVertex, startInstance);
}

extern "C" DLL_D3D11_API GraphicsContext* CreateContext()
{
	return new D3D11GraphicsContext;
}

extern "C" DLL_D3D11_API void FreeContext(GraphicsContext *context)
{
	D3D11GraphicsContext *d3d11Context = (D3D11GraphicsContext*)context;
	delete d3d11Context;
}