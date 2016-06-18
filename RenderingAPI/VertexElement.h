#pragma once

#include "PixelFormat.h"
#include "InputDataType.h"

struct VertexElement
{
	const char   *semanticName;  // not used in GL
	uint32_t      semanticIndex; // not used in GL
	PixelFormat   format;
	uint32_t      slot;          // not used in GL
	uint32_t      offset;
	InputDataType inputType;
	uint32_t      instanceDataStepRate;
};