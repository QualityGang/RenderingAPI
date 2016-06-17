#pragma once

#include <d3d11.h>

#include <AccessFlag.h>
#include <TextureFlag.h>

#include "D3D11Macros.h"
#include "D3D11DLLExport.h"

extern D3D11_MAP					Map_MapType[];
extern D3D11_PRIMITIVE_TOPOLOGY		Map_PrimitiveTopology[];
extern D3D11_FILTER					Map_TextureFilter[];
extern D3D11_TEXTURE_ADDRESS_MODE	Map_AddressMode[];
extern DXGI_FORMAT					Map_PixelFormat[];
extern D3D11_FILL_MODE				Map_FillMode[];
extern D3D11_CULL_MODE				Map_CullMode[];
extern BOOL							Map_VertexOrder[];
extern D3D11_BLEND					Map_BlendFactor[];
extern D3D11_BLEND_OP			    Map_BlendOp[];
//extern UINT Map_ColorWriteMask[]; // ColorWriteMask values in d3d11 are identical
extern D3D11_DEPTH_WRITE_MASK		Map_DepthWriteMask[];
extern D3D11_COMPARISON_FUNC	    Map_ComparisonFunc[];
extern D3D11_STENCIL_OP				Map_StencilOp[];
extern D3D11_INPUT_CLASSIFICATION	Map_InputDataType[];
extern D3D11_BIND_FLAG				Map_BufferType[];

template <typename T>
void SetAccessFlags(T *desc, uint32_t cpuAccess)
{
	desc->Usage			 = D3D11_USAGE_IMMUTABLE;
	desc->CPUAccessFlags = 0;

	if (cpuAccess & AccessFlag_Default)
		desc->Usage = D3D11_USAGE_DEFAULT;
	
	if (cpuAccess & AccessFlag_Write)
	{
		desc->Usage          = D3D11_USAGE_DYNAMIC;
		desc->CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}

	if (cpuAccess & AccessFlag_Read)
	{
		desc->Usage           = D3D11_USAGE_STAGING;
		desc->CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
	}
}

template <typename T>
void SetTextureFlags(T *desc, uint32_t flags)
{
	INCONSISTENT_FLAGS(flags, TextureFlag_RenderTarget, TextureFlag_DepthStencil);

	if (flags & TextureFlag_RenderTarget)
		desc->BindFlags = D3D11_BIND_RENDER_TARGET;
	else if (flags & TextureFlag_DepthStencil)
		desc->BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (flags & TextureFlag_ShaderResource)
		desc->BindFlags |= D3D11_BIND_SHADER_RESOURCE;
}