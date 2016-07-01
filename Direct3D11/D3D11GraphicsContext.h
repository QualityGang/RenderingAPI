#pragma once

#include <d3d11.h>

#include <GraphicsContext.h>
#include <NonCopyable.h>

#include "D3D11DLLExport.h"

class D3D11GraphicsContext : public GraphicsContext, public NonCopyable
{
public:
	DLL_D3D11_API D3D11GraphicsContext();
	DLL_D3D11_API ~D3D11GraphicsContext();
	 
	DLL_D3D11_API uint32_t getPixelFormatStride(PixelFormat format) const override;
	 
	// internal
	DLL_D3D11_API ID3D11Device*        getNativeDevice()     const { return device;     }
	DLL_D3D11_API ID3D11DeviceContext* getNativeImmContext() const { return immContext; }
	 
	DLL_D3D11_API IDXGIDevice*  getDefaultDXGIDevice() const;
	DLL_D3D11_API IDXGIAdapter* getDefaultDXGIAdapter() const;
	DLL_D3D11_API IDXGIFactory* getDefaultDXGIFactory() const;
	 
	// shaders
	DLL_D3D11_API hVertexShader createVertexShader(const void *bytecode, size_t bytecodeSize) const override;
	DLL_D3D11_API void releaseVertexShader(hVertexShader shader) const override;
	DLL_D3D11_API void setVertexShader(hVertexShader shader) const override;
	 
	DLL_D3D11_API hPixelShader createPixelShader(const void *bytecode, size_t bytecodeSize) const override;
	DLL_D3D11_API void releasePixelShader(hPixelShader shader) const override;
	DLL_D3D11_API void setPixelShader(hPixelShader shader) const override;
	 
	DLL_D3D11_API hVertexFormat createVertexFormat(
		VertexElement *elements,
		uint32_t	  numElements,
		const void	  *shaderBytecode,
		size_t		  shaderBytecodeSize
	) const override;
	DLL_D3D11_API void releaseVertexFormat(hVertexFormat format) const override;
	DLL_D3D11_API void setVertexFormat(hVertexFormat format) const override;
	
	// buffers
	DLL_D3D11_API hBuffer createBuffer(BufferType type, uint32_t size, uint32_t cpuAccess, const void *mem) const override;
	DLL_D3D11_API void releaseBuffer(hBuffer buffer) const;
	
	DLL_D3D11_API void mapBuffer(hBuffer buffer, MapType type, MapData *data) const override;
	DLL_D3D11_API void unmapBuffer(hBuffer buffer) const override;
	
	//states
	DLL_D3D11_API hRasterizerState createRasterizerState(const RasterizerState &state) const override;
	DLL_D3D11_API void releaseRasterizerState(hRasterizerState state) const override;
	DLL_D3D11_API void setRasterizerState(hRasterizerState state) const override;
	
	DLL_D3D11_API hBlendState createBlendState(const BlendState &state) const override;
	DLL_D3D11_API void releaseBlendState(hBlendState state) const override;
	DLL_D3D11_API void setBlendState(hBlendState state) const override;
	
	DLL_D3D11_API hDepthStencilState createDepthStencilState(const DepthStencilState &state) const override;
	DLL_D3D11_API void releaseDepthStencilState(hDepthStencilState state) const override;
	DLL_D3D11_API void setDepthStencilState(hDepthStencilState state) const override;
	
	DLL_D3D11_API hSamplerState createSamplerState(const SamplerState &state) const override;
	DLL_D3D11_API void releaseSamplerState(hSamplerState state) const override;
	DLL_D3D11_API void setPSSamplers(const hSamplerState *states, uint32_t startSlot, uint32_t numSamplers) const override;
	
	// textures
	DLL_D3D11_API hTexture2D createTexture2D(
		uint32_t	  width,
		uint32_t	  height,
		PixelFormat	  format,
		uint32_t	  numMipmaps,
		uint32_t	  numSamples,
		uint32_t	  texFlags,
		uint32_t	  cpuAccess,
		void	      *mem,
		uint32_t	  rowPitch
	) const override;
	DLL_D3D11_API void releaseTexture2D(hTexture2D texture) const override;
	
	DLL_D3D11_API void getTexture2DSize(hTexture2D texture, TextureSize *texSize) const override;
	
	DLL_D3D11_API void mapTexture2D(hTexture2D texture, MapType type, MapData *data) const override;
	DLL_D3D11_API void unmapTexture2D(hTexture2D texture) const override;
	
	// resource manipulation
	DLL_D3D11_API void copyBuffer(hBuffer src, hBuffer dst) const override;
	DLL_D3D11_API void copyTexture2D(hTexture2D src, hTexture2D dst) const override;

	// render target
	DLL_D3D11_API hRenderTarget createRenderTarget(const RenderTarget &renderTarget) const override;
	DLL_D3D11_API void releaseRenderTarget(hRenderTarget renderTarget) const override;
	
	DLL_D3D11_API hTexture2D getTexture2D(hRenderTarget renderTarget, uint32_t slot) const override;

	// render window
	DLL_D3D11_API hRenderWindow createRenderWindow(
		HWND		hwnd,
		uint32_t	width,
		uint32_t	height,
		uint32_t	bufferCount,
		PixelFormat bufferFormat,
		uint32_t    numSamples,
		bool        vsync
	) const override;
	DLL_D3D11_API void releaseRenderWindow(hRenderWindow window) const override;
	
	DLL_D3D11_API void setVSyncEnabled(hRenderWindow window, bool vsync) const override;
	DLL_D3D11_API bool isVSyncEnabled(hRenderWindow window) const override;
	DLL_D3D11_API hTexture2D getBackBuffer(hRenderWindow window) const override;
	DLL_D3D11_API void swapBuffers(hRenderWindow window) const override;
	DLL_D3D11_API hRenderTarget getRenderTarget(hRenderWindow window) const override;
	DLL_D3D11_API void resize(hRenderWindow window, uint32_t width, uint32_t height) const override;
	
	// drawing
	DLL_D3D11_API void setViewport(float width, float height, float minDepth, float maxDepth) const override;
	
	DLL_D3D11_API void setRenderTarget(hRenderTarget renderTarget) const override;
	DLL_D3D11_API void clearAllRenderTargets(hRenderTarget renderTarget, const Color &color) const override;
	DLL_D3D11_API void clearRenderTarget(hRenderTarget renderTarget, uint32_t slot, const Color &color) const override;
	DLL_D3D11_API void clearDepthStencil(hRenderTarget renderTarget, float depth, uint8_t stencil) const override;

	DLL_D3D11_API void setPrimitiveTopology(PrimitiveTopology topology) const override;
	DLL_D3D11_API void setVertexBuffers(const hBuffer *buffers, uint32_t startSlot, uint32_t numBuffers, const uint32_t *strides, const uint32_t *offsets) const override;
	DLL_D3D11_API void setIndexBuffer(hBuffer buffer, uint32_t offset) const override;
	
	DLL_D3D11_API void setPSConstantBuffers(const hBuffer *buffers, uint32_t startSlot, uint32_t numBuffers) const override;
	DLL_D3D11_API void setPSTexture2Ds(const hTexture2D *textures, uint32_t startSlot, uint32_t numTextures) const override;
	
	DLL_D3D11_API void draw(uint32_t count, uint32_t startVertex) const override;
	DLL_D3D11_API void drawIndexed(uint32_t count, uint32_t startIndex, uint32_t startVertex) const override;
	
	DLL_D3D11_API void drawInstanced(uint32_t instanceCount, uint32_t count, uint32_t startVertex, uint32_t startInstance) const override;
	DLL_D3D11_API void drawIndexedInstanced(uint32_t instanceCount, uint32_t count, uint32_t startIndex, uint32_t startVertex, uint32_t startInstance) const override;
private:
	ID3D11Device *device;
	ID3D11DeviceContext *immContext;
};

