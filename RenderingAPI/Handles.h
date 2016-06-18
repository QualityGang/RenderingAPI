#pragma once

#include <stdint.h>

template <typename T, uint32_t Id>
struct Handle
{
	Handle() : value(0) {}
	explicit Handle(const T &value) : value(value) {}
	Handle(std::nullptr_t n) : value(n) {}
	 
	template <typename U>
	U* as() const { return (U*)value; }
	 
	void operator=(std::nullptr_t n) { value = n; }
	 
	bool operator==(const Handle &h) { return value == h.value; }
	bool operator!=(const Handle &h) { return value != h.value; }
	 
	operator bool() const { return value != 0; }
protected:
	T value;
};

typedef Handle<void*, 0>  hUnknown;
typedef Handle<void*, 1>  hVertexShader;
typedef Handle<void*, 2>  hHullShader;
typedef Handle<void*, 3>  hDomainShader;
typedef Handle<void*, 4>  hGeometryShader;
typedef Handle<void*, 5>  hPixelShader;
typedef Handle<void*, 6>  hComputeShader;
typedef Handle<void*, 7>  hVertexFormat;
typedef Handle<void*, 8>  hRasterizerState;
typedef Handle<void*, 9>  hBlendState;
typedef Handle<void*, 10> hDepthStencilState;
typedef Handle<void*, 11> hSamplerState;
typedef Handle<void*, 12> hTexture1D;
typedef Handle<void*, 13> hTexture2D;
typedef Handle<void*, 14> hTexture3D;
typedef Handle<void*, 15> hCubemap;
typedef Handle<void*, 16> hRenderTarget;
typedef Handle<void*, 17> hBuffer;
typedef Handle<void*, 18> hRenderWindow;