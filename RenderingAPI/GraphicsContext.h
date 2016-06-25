#pragma once

#include <Windows.h>

#include "Color.h"
#include "Handles.h"
#include "BufferType.h"
#include "AccessFlag.h"
#include "MapType.h"
#include "MapData.h"
#include "PixelFormat.h"
#include "VertexElement.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "SamplerState.h"
#include "TextureFlag.h"
#include "TextureSize.h"
#include "RenderTarget.h"
#include "PrimitiveTopology.h"
#include "DLLExport.h"

// TODO: 
//  - MSAA
//  - fullscreen
//	- other shaders apart from VS/PS
//  - compile shaders
//  - 1D/2D textures
//  - indirect drawing
//  - remove GL/D3D inconsistencies
//  - resource manipulation

class GraphicsContext
{
public:
	DLL_REN_API GraphicsContext();
	DLL_REN_API ~GraphicsContext();

	virtual uint32_t getPixelFormatStride(PixelFormat format) const = 0;

	// shaders
	virtual hVertexShader createVertexShader(const void *bytecode, size_t bytecodeSize) const = 0;
	virtual void releaseVertexShader(hVertexShader shader) const = 0;
	virtual void setVertexShader(hVertexShader shader) const = 0;

	virtual hPixelShader createPixelShader(const void *bytecode, size_t bytecodeSize) const = 0;
	virtual void releasePixelShader(hPixelShader shader) const = 0;
	virtual void setPixelShader(hPixelShader shader) const = 0;

	virtual hVertexFormat createVertexFormat(
		VertexElement *elements,
		uint32_t	  numElements,
		const void	  *shaderBytecode,
		size_t		  shaderBytecodeSize
	) const = 0;
	virtual void releaseVertexFormat(hVertexFormat format) const = 0;
	virtual void setVertexFormat(hVertexFormat format) const = 0;

	// buffers
	virtual hBuffer createBuffer(BufferType type, uint32_t size, uint32_t cpuAccess, const void *mem) const = 0;
	virtual void releaseBuffer(hBuffer buffer) const = 0;

	virtual void mapBuffer(hBuffer buffer, MapType type, MapData *data) const = 0;
	virtual void unmapBuffer(hBuffer buffer) const = 0;

	// states
	virtual hRasterizerState createRasterizerState(const RasterizerState &state) const = 0;
	virtual void releaseRasterizerState(hRasterizerState state) const = 0;
	virtual void setRasterizerState(hRasterizerState state) const = 0;

	virtual hBlendState createBlendState(const BlendState &state) const = 0;
	virtual void releaseBlendState(hBlendState state) const = 0;
	virtual void setBlendState(hBlendState state) const = 0;
	
	virtual hDepthStencilState createDepthStencilState(const DepthStencilState &state) const = 0;
	virtual void releaseDepthStencilState(hDepthStencilState state) const = 0;
	virtual void setDepthStencilState(hDepthStencilState state) const = 0;
	
	virtual hSamplerState createSamplerState(const SamplerState &state) const = 0;
	virtual void releaseSamplerState(hSamplerState state) const = 0;
	virtual void setPSSamplers(const hSamplerState *states, uint32_t startSlot, uint32_t numSamplers) const = 0;

	// textures
	virtual hTexture2D createTexture2D(
		uint32_t	  width,
		uint32_t	  height,
		PixelFormat	  format,
		uint32_t	  numMipmaps,
		uint32_t	  numSamples,
		uint32_t	  texFlags,
		uint32_t      cpuAccess,
		void	      *mem,
		uint32_t	  rowPitch
	) const = 0;
	virtual void releaseTexture2D(hTexture2D texture) const = 0;

	virtual void getTexture2DSize(hTexture2D texture, TextureSize *texSize) const = 0;

	virtual void mapTexture2D(hTexture2D texture, MapType type, MapData *data) const = 0;
	virtual void unmapTexture2D(hTexture2D texture) const = 0;

	// render target
	virtual hRenderTarget createRenderTarget(const RenderTarget &renderTarget) const = 0;
	virtual void releaseRenderTarget(hRenderTarget renderTarget) const = 0;

	virtual hTexture2D getTexture2D(hRenderTarget renderTarget, uint32_t slot) const = 0;

	// render window
	virtual hRenderWindow createRenderWindow(
		HWND		hwnd,
		uint32_t	width,
		uint32_t	height,
		uint32_t	bufferCount,
		PixelFormat bufferFormat,
		uint32_t    numSamples,
		bool        vsync
	) const = 0;
	virtual void releaseRenderWindow(hRenderWindow window) const = 0;

	virtual void setVSyncEnabled(hRenderWindow window, bool vsync) const = 0;
	virtual bool isVSyncEnabled(hRenderWindow window) const = 0;
	virtual hTexture2D getBackBuffer(hRenderWindow window) const = 0;
	virtual void swapBuffers(hRenderWindow window) const = 0;
	virtual hRenderTarget getRenderTarget(hRenderWindow window) const = 0;
	virtual void resize(hRenderWindow window, uint32_t width, uint32_t height) const = 0;

	// drawing
	virtual void setViewport(float width, float height, float minDepth, float maxDepth) const = 0;
	
	virtual void setRenderTarget(hRenderTarget renderTarget) const = 0;
	virtual void clearAllRenderTargets(hRenderTarget renderTarget, const Color &color) const = 0;
	virtual void clearRenderTarget(hRenderTarget renderTarget, uint32_t slot, const Color &color) const = 0;
	virtual void clearDepthStencil(hRenderTarget renderTarget, float depth, uint8_t stencil) const = 0;
	
	virtual void setPrimitiveTopology(PrimitiveTopology topology) const = 0;
	virtual void setVertexBuffers(const hBuffer *buffers, uint32_t startSlot, uint32_t numBuffers, const uint32_t *strides, const uint32_t *offsets) const = 0;
	virtual void setIndexBuffer(hBuffer buffer, uint32_t offset) const = 0;
	
	virtual void setPSConstantBuffers(const hBuffer *buffers, uint32_t startSlot, uint32_t numBuffers) const = 0;
	virtual void setPSTexture2Ds(const hTexture2D *textures, uint32_t startSlot, uint32_t numTextures) const = 0;
	
	virtual void draw(uint32_t count, uint32_t startVertex) const = 0;
	virtual void drawIndexed(uint32_t count, uint32_t startIndex, uint32_t startVertex) const = 0;
	
	virtual void drawInstanced(uint32_t instanceCount, uint32_t count, uint32_t startVertex, uint32_t startInstance) const = 0;
	virtual void drawIndexedInstanced(uint32_t instanceCount, uint32_t count, uint32_t startIndex, uint32_t startVertex, uint32_t startInstance) const = 0;

	hBlendState BSAlphaBlend;
	hBlendState BSAdditive;
	//hBlendState BSNonPremultiplied;
	hBlendState BSOpaque;

	hDepthStencilState DSSDefault;
	hDepthStencilState DSSRead;
	hDepthStencilState DSSNone;

	hRasterizerState RSCullClockwise;
	hRasterizerState RSCullCounterClockwise;
	hRasterizerState RSCullNone;
	hRasterizerState RSWireframe;

	hSamplerState SSPointWrap;
	hSamplerState SSPointClamp;
	hSamplerState SSLinearWrap;
	hSamplerState SSLinearClamp;
	hSamplerState SSAnisotropicWrap;
	hSamplerState SSAnisotropicClamp;
protected:
	DLL_REN_API void init(uint32_t maxAnisotropy);
	DLL_REN_API void dispose();
};
