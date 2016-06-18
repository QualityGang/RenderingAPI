#pragma once

#include <stdint.h>

struct MapData
{
	MapData() {}
	MapData(void *mem, uint32_t rowPitch, uint32_t depthPitch)
		: mem(mem), rowPitch(rowPitch), depthPitch(depthPitch) {}

	void       *mem       = nullptr;
	uint32_t   rowPitch   = 0;
	uint32_t   depthPitch = 0;
};
