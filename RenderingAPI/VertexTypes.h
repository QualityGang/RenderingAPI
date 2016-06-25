#pragma once

#include <DirectXMath.h>

#include "AlignedNew.h"

ALIGN(16)
struct VertexPosTexColor : public AlignedNew<16>
{
	VertexPosTexColor(float x, float y, float z,
		float u, float v,
		float r, float g, float b, float a)
		: position(x, y, z, 1.0f), texCoords(u, v), color(r, g, b, a)
	{
	}

	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT2 texCoords;
	DirectX::XMFLOAT4 color;
};

ALIGN(16)
struct VertexPosColor : public AlignedNew<16>
{
	VertexPosColor(float x, float y, float z,
		float r, float g, float b, float a)
		: position(x, y, z, 1.0f), color(r, g, b, a)
	{
	}

	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
};