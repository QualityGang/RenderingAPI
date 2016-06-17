#pragma once

#include "ComparisonFunc.h"
#include "StencilOp.h"

struct StencilDesc
{
	ComparisonFunc stencilFunc = ComparisonFunc_Always;
	StencilOp      failOp	   = StencilOp_Keep;
	StencilOp      depthFailOp = StencilOp_Keep;
	StencilOp      passOp      = StencilOp_Keep;
};