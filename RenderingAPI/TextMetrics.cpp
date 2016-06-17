#include "TextMetrics.h"

#include <DirectXMath.h>

#include "SpriteBatch.h"
#include "stringex.h"

using namespace DirectX;

int TextMetrics::GetMaxAscent(const char *str, const FontAtlas &atlas)
{
	int32_t ascent = 0;

	for (const char *lpStr = str; *lpStr; lpStr++)
	{
		const FontAtlas::CharInfo *charInfo = atlas.getCharInfo(*lpStr);

		if (charInfo && charInfo->ascent > ascent)
			ascent = charInfo->ascent;
	}

	return ascent;
}

int TextMetrics::GetMaxDescent(const char *str, const FontAtlas &atlas)
{
	int32_t descent = 0;

	for (const char *lpStr = str; *lpStr; lpStr++)
	{
		const FontAtlas::CharInfo *charInfo = atlas.getCharInfo(*lpStr);

		if (charInfo && charInfo->descent > descent)
			descent = charInfo->descent;
	}

	return descent;
}

void TextMetrics::GetSize(const Text &text, const FontAtlas &atlas, float *width, float *height)
{
	const Font *font = atlas.getFont();

	if (!font)
		return;

	FT_Face face = font->getFace();

	FT_UInt prevGlyphIndex = 0;
	XMFLOAT2 pen(0, 0);

	std::vector<std::string> lines;
	stdex::split_string(text.str, "\n", &lines);

	uint32_t currLine = 0;
	int32_t maxAscent = GetMaxAscent(lines[currLine].c_str(), atlas);
	int32_t maxDescent = GetMaxDescent(lines[currLine].c_str(), atlas);

	for (const char *lpStr = text.str; *lpStr; lpStr++)
	{
		switch (*lpStr)
		{
			case ' ':
				pen.x += atlas.getCharInfo(' ')->advance.x;
				continue;
			case '\t':
				pen.x += atlas.getCharInfo(' ')->advance.x * 3;
				continue;
			case '\n':
				pen.x = 0;
				pen.y += text.lineGap + maxDescent + maxAscent;
				currLine++;
				maxAscent = GetMaxAscent(lines[currLine].c_str(), atlas);
				maxDescent = GetMaxDescent(lines[currLine].c_str(), atlas);
				continue;
		}

		const FontAtlas::CharInfo *charInfo = atlas.getCharInfo(*lpStr);

		if (!charInfo)
		{
			pen.x += atlas.getCharInfo(' ')->advance.x;
			continue;
		}

		FT_UInt glyphIndex = FT_Get_Char_Index(face, *lpStr);
		pen.x += font->getKerning(prevGlyphIndex, glyphIndex);

		if (!*(lpStr + 1))
		{
			if (width)  *width  = text.position.x + pen.x + charInfo->rect.x + charInfo->rect.width;
			if (height) *height = text.position.y + pen.y + maxAscent - charInfo->rect.y + charInfo->rect.height;
		}
		else
		{
			pen.x += charInfo->advance.x;
			prevGlyphIndex = glyphIndex;
		}
	}
}
