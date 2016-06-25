#pragma once

#include <stdint.h>

enum PrimitiveTopology : uint32_t
{
	PrimitiveTopology_PointList,
	PrimitiveTopology_LineList,
	PrimitiveTopology_LineStrip,
	PrimitiveTopology_TriangleList,
	PrimitiveTopology_TriangleStrip
};