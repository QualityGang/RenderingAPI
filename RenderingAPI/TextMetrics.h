#pragma once

#include "FontAtlas.h"
#include "DLLExport.h"

struct Text;

class TextMetrics
{
public:
	TextMetrics() = delete;
	~TextMetrics() = delete;

	DLL_REN_API static int GetMaxAscent(const char *str, const FontAtlas &atlas);
	DLL_REN_API static int GetMaxDescent(const char *str, const FontAtlas &atlas);
	DLL_REN_API static void GetSize(const Text &text, const FontAtlas &atlas, float *width, float *height);
};

