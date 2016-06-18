#pragma once

#include <stdint.h>

enum PrimitiveTopology : uint32_t
{
	PrimitiveTopology_TriangleList,
	PrimitiveTopology_TriangleStrip,
	PrimitiveTopology_PointList
};