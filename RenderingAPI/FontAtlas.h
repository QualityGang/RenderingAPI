#pragma once

#include <DirectXMath.h>
#include <unordered_map>

#include "Bitmap.h"
#include "Font.h"
#include "Rect.h"

#define FA_ALLLOWERCASE "abcdefghijklmnopqrstuvwxyz"
#define FA_ALLUPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define FA_ALLNUMBERS   "1234567890"
//#define FA_ALLSYMBOLS   ""

class FontAtlas : public NonCopyable
{
public:
	DLL_REN_API FontAtlas(GraphicsContext *context, const char *chars, const Font &font);
	DLL_REN_API ~FontAtlas();

	struct CharInfo
	{
		DirectX::XMUINT2 advance;
		IntRect rect;
		int ascent;
		int descent;
		DirectX::XMUINT2 charPos;
	};

	DLL_REN_API const CharInfo* getCharInfo(char c) const;

	const Font*   getFont()   const { return font;   }
	uint32_t      getWidth()  const { return width;  }
	uint32_t      getHeight() const { return height; }
	const Bitmap& getBitmap() const { return bitmap; }
private:
	static bool LoadChar(FT_Face face, FT_ULong charCode);

	const Font *font;
	uint32_t width;
	uint32_t height;
	Bitmap bitmap;
	mutable std::unordered_map<char, CharInfo> charMap;
};
