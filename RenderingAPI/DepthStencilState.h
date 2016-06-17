#pragma once

#include "DepthWriteMask.h"
#include "StencilDesc.h"

struct DepthStencilState
{
	bool           depthEnabled   = true;
	DepthWriteMask writeMask      = DepthWriteMask_All;
	ComparisonFunc depthFunc      = ComparisonFunc_LessEqual;

	bool           stencilEnabled		= false;
	uint32_t       stencilRef			= 0;
	uint8_t        stencilReadMask		= 0;
	uint8_t        stencilWriteMask		= 0;
	StencilDesc    frontFaceStencilDesc;
	StencilDesc    backFaceStencilDesc;
};