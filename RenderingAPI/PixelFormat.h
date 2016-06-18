#pragma once

#include <stdint.h>

enum PixelFormat : uint32_t
{
	PixelFormat_BC1,    // DXT1
	PixelFormat_BC2,    // DXT3
	PixelFormat_BC3,    // DXT5
	PixelFormat_BC4,    // LATC1/ATI1
	PixelFormat_BC5,    // LATC2/ATI2
	PixelFormat_BC6H,   // BC6H
	PixelFormat_BC7,    // BC7
	PixelFormat_ETC1,   // ETC1 RGB8
	PixelFormat_ETC2,   // ETC2 RGB8
	PixelFormat_ETC2A,  // ETC2 RGBA8
	PixelFormat_ETC2A1, // ETC2 RGB8A1
	PixelFormat_PTC12,  // PVRTC1 RGB  2BPP
	PixelFormat_PTC14,  // PVRTC1 RGB  4BPP
	PixelFormat_PTC12A, // PVRTC1 RGBA 2BPP
	PixelFormat_PTC14A, // PVRTC1 RGBA 4BPP
	PixelFormat_PTC22,  // PVRTC2 RGBA 2BPP
	PixelFormat_PTC24,  // PVRTC2 RGBA 4BPP
	
	PixelFormat_UnknownCompressed, // compressed formats above
	
	PixelFormat_R1,
	PixelFormat_R8,
	PixelFormat_R16,
	PixelFormat_R16F,
	PixelFormat_R32I,
	PixelFormat_R32U,
	PixelFormat_R32F,
	PixelFormat_RG8,
	PixelFormat_RG16,
	PixelFormat_RG16F,
	PixelFormat_RG32I,
	PixelFormat_RG32U,
	PixelFormat_RG32F,
	PixelFormat_RGB32I,
	PixelFormat_RGB32U,
	PixelFormat_RGB32F,
	PixelFormat_RGBA8,
	PixelFormat_RGBA16,
	PixelFormat_RGBA16F,
	PixelFormat_RGBA32I,
	PixelFormat_RGBA32U,
	PixelFormat_RGBA32F,
	PixelFormat_R11G11B10F,
	
	PixelFormat_UnknownDepth, // depth formats below
	
	PixelFormat_D16,
	PixelFormat_D24S8,
	PixelFormat_D32F
};

inline bool IsCompressedFormat(PixelFormat format) 
{ 
	return format <= PixelFormat_UnknownCompressed;
}

inline bool IsDepthFormat(PixelFormat format) 
{
	return format >= PixelFormat_UnknownDepth;
}