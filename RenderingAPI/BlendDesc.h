#pragma once

#include "ColorWriteMask.h"
#include "BlendFactor.h"
#include "BlendOp.h"

struct BlendDesc
{
	bool           blendEnabled  = false;
	uint32_t	   writeMask     = ColorWriteMask_All;
	BlendFactor    srcBlend      = BlendFactor_One;
	BlendFactor    dstBlend      = BlendFactor_Zero;
	BlendOp        blendOp       = BlendOp_Add;
	BlendFactor    srcBlendAlpha = BlendFactor_One;
	BlendFactor    dstBlendAlpha = BlendFactor_Zero;
	BlendOp        blendOpAlpha  = BlendOp_Add;
};