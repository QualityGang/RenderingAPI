#pragma once

#include <stdint.h>

enum TextureFlag : uint32_t
{
	TextureFlag_RenderTarget   = (1U << 0),
	TextureFlag_DepthStencil   = (1U << 1),
	TextureFlag_ShaderResource = (1U << 2)
};
