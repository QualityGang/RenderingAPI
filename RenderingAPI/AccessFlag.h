#pragma once

#include <stdint.h>

enum AccessFlag : uint32_t
{
	AccessFlag_None    = 0,
	AccessFlag_Default = (1U << 0),
	AccessFlag_Read    = (1U << 1),
	AccessFlag_Write   = (1U << 2)
};
