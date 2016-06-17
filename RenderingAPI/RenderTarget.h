#pragma once

#include "Handles.h"

struct RenderTarget
{
	uint32_t numColorTextures = 0;
	hTexture2D colorTextures[8];
	hTexture2D depthStencilTexture;
};
