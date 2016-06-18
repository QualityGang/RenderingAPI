#pragma once

#include <float.h>

#include "TextureFilter.h"
#include "AddressMode.h"
#include "ComparisonFunc.h"

struct SamplerState
{
	TextureFilter   filter   = TextureFilter_MinMagMip_Linear;
	AddressMode     addressU = AddressMode_Clamp;
	AddressMode     addressV = AddressMode_Clamp;
	AddressMode     addressW = AddressMode_Clamp;
	float           lodBias  = 0.0f;
	uint32_t        maxAnisotropy = 1;
	ComparisonFunc  comparisonFunc = ComparisonFunc_Never;
	Color           borderColor;
	float           minLod = -FLT_MAX;
	float           maxLod =  FLT_MAX;
};