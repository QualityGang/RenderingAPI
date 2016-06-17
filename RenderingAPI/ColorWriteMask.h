#pragma once

#include <stdint.h>

enum ColorWriteMask : uint32_t
{
	ColorWriteMask_Red   = (1U << 0),
	ColorWriteMask_Green = (1U << 1),
	ColorWriteMask_Blue  = (1U << 2),
	
	ColorWriteMask_All = ColorWriteMask_Red | ColorWriteMask_Green | ColorWriteMask_Blue
};
