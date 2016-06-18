#include "FontAtlas.h"

#include <algorithm>
#include <unordered_set>

#include "Throw.h"

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

bool FontAtlas::LoadChar(FT_Face face, FT_ULong charCode)
{
	FT_Error error = FT_Load_Char(face, charCode, FT_LOAD_RENDER);

	if (error)
		return false;

	return true;
}