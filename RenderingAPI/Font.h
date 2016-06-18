#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdint.h>

#include "NonCopyable.h"
#include "DLLExport.h"

class Font : public NonCopyable
{
public:
	DLL_REN_API Font(const char *filename, uint16_t size);
	DLL_REN_API ~Font();

	DLL_REN_API void setSize(uint16_t size);

	FT_Face  getFace() const { return face; }
	uint16_t getSize() const
	{
		return face ? face->size->metrics.x_ppem : 0;
	}
	
	DLL_REN_API int32_t getKerning(uint32_t leftIndex, uint32_t rightIndex) const;
private:
	FT_Face face = nullptr;
};

