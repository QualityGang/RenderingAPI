#include "FontAtlas.h"

#include <algorithm>
#include <unordered_set>

#include <DirectXMath.h>

#include "stringex.h"
#include "Throw.h"

using namespace DirectX;

#define GAP_SIZE 5 

FontAtlas::FontAtlas(GraphicsContext *context, const char *chars, const Font &font) :
	bitmap(context), font(&font), width(GAP_SIZE)
{
	FT_Face face = font.getFace();
	FT_GlyphSlot glyph = face->glyph;

	width += glyph->bitmap.width + GAP_SIZE;
	height =  glyph->bitmap.rows;

	std::unordered_set<char> charSet(chars, chars + strlen(chars));
	charSet.insert(' ');

	for (const char c : charSet)
	{
		if (!LoadChar(face, c))
			continue;

		width += glyph->bitmap.width + GAP_SIZE;
		height = std::max(height, glyph->bitmap.rows);
	}

	height += 2 * GAP_SIZE;

	uint32_t x = GAP_SIZE;
	uint32_t y = GAP_SIZE;

	bitmap.create(width, height);

	for (const char c : charSet)
	{
		if (!LoadChar(face, c))
			continue;

		for (uint32_t iy = 0; iy < glyph->bitmap.rows; iy++)
		{
			for (uint32_t ix = 0; ix < glyph->bitmap.width; ix++)
			{
				uint32_t index = iy * glyph->bitmap.pitch + ix;
				bitmap.setPixel(x + ix, y + iy, Color(255, 255, 255, glyph->bitmap.buffer[index]));
			}
		}

		CharInfo charInfo;
		charInfo.advance.x = glyph->advance.x >> 6;
		charInfo.advance.y = glyph->advance.y >> 6;
		charInfo.rect.x = glyph->bitmap_left;
		charInfo.rect.y = glyph->bitmap_top;
		charInfo.rect.width  = glyph->bitmap.width;
		charInfo.rect.height = glyph->bitmap.rows;
		charInfo.ascent  = glyph->metrics.horiBearingY >> 6;
		charInfo.descent = (glyph->metrics.height - glyph->metrics.horiBearingY) >> 6;
		charInfo.charPos.x = x;
		charInfo.charPos.y = y;

		charMap[c] = charInfo;
		x += glyph->bitmap.width + GAP_SIZE;
	}

	if (!getCharInfo(' '))
		THROW("Space character not loaded");
}

FontAtlas::~FontAtlas()
{
}

const FontAtlas::CharInfo* FontAtlas::getCharInfo(char c) const
{
	auto iter = charMap.find(c);

	if (iter == charMap.end())
		return nullptr;

	return &iter->second;
}

int FontAtlas::getMaxAscent(const char *str) const
{
	int32_t ascent = 0;

	for (const char *lpStr = str; *lpStr; lpStr++)
	{
		const FontAtlas::CharInfo *charInfo = getCharInfo(*lpStr);

		if (charInfo && charInfo->ascent > ascent)
			ascent = charInfo->ascent;
	}

	return ascent;
}

int FontAtlas::getMaxDescent(const char *str) const
{
	int32_t descent = 0;

	for (const char *lpStr = str; *lpStr; lpStr++)
	{
		const FontAtlas::CharInfo *charInfo = getCharInfo(*lpStr);

		if (charInfo && charInfo->descent > descent)
			descent = charInfo->descent;
	}

	return descent;
}

void FontAtlas::getTextSize(const Text &text, float *width, float *height)
{
	if (!font)
		return;

	FT_Face face = font->getFace();

	FT_UInt prevGlyphIndex = 0;
	XMFLOAT2 pen(0, 0);

	std::vector<std::string> lines;
	stdex::split_string(text.getString(), "\n", &lines);

	uint32_t currLine  = 0;
	int32_t maxAscent  = getMaxAscent(lines[currLine].c_str());
	int32_t maxDescent = getMaxDescent(lines[currLine].c_str());

	for (const char *lpStr = text.getString(); *lpStr; lpStr++)
	{
		switch (*lpStr)
		{
			case ' ':
				pen.x += getCharInfo(' ')->advance.x;
				continue;
			case '\t':
				pen.x += getCharInfo(' ')->advance.x * 3;
				continue;
			case '\n':
				pen.x = 0;
				pen.y += text.getLineGap() + maxDescent + maxAscent;
				currLine++;
				maxAscent  = getMaxAscent(lines[currLine].c_str());
				maxDescent = getMaxDescent(lines[currLine].c_str());
				continue;
		}

		const FontAtlas::CharInfo *charInfo = getCharInfo(*lpStr);

		if (!charInfo)
		{
			pen.x += getCharInfo(' ')->advance.x;
			continue;
		}

		FT_UInt glyphIndex = FT_Get_Char_Index(face, *lpStr);
		pen.x += font->getKerning(prevGlyphIndex, glyphIndex);

		if (!*(lpStr + 1))
		{
			if (width)  *width = text.getX() + pen.x + charInfo->rect.x + charInfo->rect.width;
			if (height) *height = text.getY() + pen.y + maxAscent - charInfo->rect.y + charInfo->rect.height;
		}
		else
		{
			pen.x += charInfo->advance.x;
			prevGlyphIndex = glyphIndex;
		}
	}
}

bool FontAtlas::LoadChar(FT_Face face, FT_ULong charCode)
{
	FT_Error error = FT_Load_Char(face, charCode, FT_LOAD_RENDER);

	if (error)
		return false;

	return true;
}