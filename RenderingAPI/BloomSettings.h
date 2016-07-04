#pragma once

#include "DLLExport.h"

struct BloomSettings
{
	// Controls how bright a pixel needs to be before it will bloom.
	// Zero makes everything bloom equally, while higher values select
	// only brighter colors. Somewhere between 0.25 and 0.5 is good.
	float bloomThreshold;

	// Controls how much blurring is applied to the bloom image.
	// The typical range is from 1 up to 10 or so.
	float blurAmount;

	// Controls the amount of the bloom and base images that
	// will be mixed into the final scene. Range 0 to 1.
	float bloomIntensity;
	float baseIntensity;

	// Independently control the color saturation of the bloom and
	// base images. Zero is totally desaturated, 1.0 leaves saturation
	// unchanged, while higher values increase the saturation level.
	float bloomSaturation;
	float baseSaturation;
	
	// Table of preset bloom settings.
	DLL_REN_API static const BloomSettings Default;
	DLL_REN_API static const BloomSettings Soft;
	DLL_REN_API static const BloomSettings Desaturated;
	DLL_REN_API static const BloomSettings Saturated;
	DLL_REN_API static const BloomSettings Blurry;
	DLL_REN_API static const BloomSettings Subtle;
};
