#pragma once

#include <stdint.h>

enum BlendFactor : uint32_t
{
	BlendFactor_Zero,
	BlendFactor_One,
	BlendFactor_SrcAlpha,
	BlendFactor_DstAlpha,
	BlendFactor_OneMinusSrcAlpha,
	BlendFactor_OneMinusDstAlpha,
	BlendFactor_SrcColor,
	BlendFactor_DstColor,
	BlendFactor_OneMinusSrcColor,
	BlendFactor_OneMinusDstColor
};