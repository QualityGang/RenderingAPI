#pragma once

#include <stdint.h>

enum TextureFilter : uint32_t
{
	TextureFilter_MinMagMip_Point,
	TextureFilter_MinMag_Point_Mip_Linear,
	TextureFilter_Min_Point_Mag_Linear_Mip_Point,
	TextureFilter_Min_Point_MagMip_Linear,
	TextureFilter_Min_Linear_MagMip_Point,
	TextureFilter_Min_Linear_Mag_Point_Mip_Linear,
	TextureFilter_MinMag_Linear_Mip_Point,
	TextureFilter_MinMagMip_Linear,
	TextureFilter_Anisotropic
};

