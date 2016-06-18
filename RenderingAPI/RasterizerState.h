#pragma once

#include "FillMode.h"
#include "CullMode.h"
#include "VertexOrder.h"

struct RasterizerState
{
	FillMode	fillMode    = FillMode_Solid;
	CullMode	cullMode    = CullMode_Back;
	VertexOrder vertexOrder = VertexOrder_Clockwise;
};