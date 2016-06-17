#pragma once

#include <stdint.h>

enum StencilOp : uint32_t
{
	StencilOp_Keep,
	StencilOp_Zero,
	StencilOp_Replace,
	StencilOp_Increment,
	StencilOp_Decrement
};