#pragma once

#include <stdint.h>

enum ComparisonFunc : uint32_t
{
	ComparisonFunc_Always,
	ComparisonFunc_Never,
	ComparisonFunc_Less,
	ComparisonFunc_LessEqual,
	ComparisonFunc_Greater,
	ComparisonFunc_GreaterEqual,
	ComparisonFunc_Equal,
	ComparisonFunc_NotEqual
};
