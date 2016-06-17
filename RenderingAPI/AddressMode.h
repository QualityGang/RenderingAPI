#pragma once

#include <stdint.h>

enum AddressMode : uint32_t
{
	AddressMode_Wrap,
	AddressMode_Mirror,
	AddressMode_Clamp,
	AddressMode_Border
};
