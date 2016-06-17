#pragma once

#include <stdint.h>

enum CullMode : uint32_t
{
	CullMode_Back,
	CullMode_Front,
	CullMode_None
};