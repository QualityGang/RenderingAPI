#pragma once

#include "BlendDesc.h"

struct BlendState
{
	bool      separateBlendEnabled = false;
	BlendDesc blendDesc[8];
	bool      alphaToCoverageEnabled = false;
};