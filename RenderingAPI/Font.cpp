#include "Font.h"

Font::Font(const char *filename, uint16_t size)
{
	static struct FT
	{
		FT::FT()
		{
			FT_Init_FreeType(&library);
		}

		FT::~FT()
		{
			FT_Done_FreeType(library);
		}
		
		FT_Library library = nullptr;
	} ft;

	FT_New_Face(ft.library, filename, 0, &face);
	setSize(size);
}

Font::~Font()
{
	FT_Done_Face(face);
}

void Font::setSize(uint16_t size)
{
	if (size != getSize())
		FT_Set_Pixel_Sizes(face, 0, size);
}

int32_t Font::getKerning(uint32_t leftIndex, uint32_t rightIndex) const
{
	FT_Bool useKerning = FT_HAS_KERNING(face);

	if (useKerning && leftIndex && rightIndex)
	{
		FT_Vector delta;

		FT_Error error = FT_Get_Kerning(
			face,
			leftIndex,
			rightIndex,
			FT_KERNING_DEFAULT,
			&delta
		);

		if (!error)
			return delta.x >> 6;
	}

	return 0;
}