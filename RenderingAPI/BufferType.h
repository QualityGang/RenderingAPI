#pragma once

#include <stdint.h>

enum BufferType : uint32_t
{
	BufferType_VertexBuffer,
	BufferType_IndexBuffer,
	BufferType_ConstantBuffer
};
