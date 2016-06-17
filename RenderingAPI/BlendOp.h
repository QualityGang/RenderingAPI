#pragma once

#include <stdint.h>

enum BlendOp : uint32_t
{
	BlendOp_Add,
	BlendOp_Subtract,
	BlendOp_RevSubtract,
	BlendOp_Min,
	BlendOp_Max
};