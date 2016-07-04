#include "Bitmap.h"

extern "C" DLL_REN_API Bitmap* Bitmap_Create(GraphicsContext *context, uint32_t width, uint32_t height)
{
	return new Bitmap(context, width, height);
}

extern "C" DLL_REN_API Bitmap* Bitmap_CreateFromFile(GraphicsContext *context, const char *fileName)
{
	return new Bitmap(context, fileName);
}

extern "C" DLL_REN_API void Bitmap_Release(Bitmap *bmp)
{
	delete bmp;
}

extern "C" DLL_REN_API void Bitmap_SetPixel_I(Bitmap *bmp, uint32_t index, Color color)
{
	bmp->setPixel(index, color);
}

extern "C" DLL_REN_API void Bitmap_SetPixel_XY(Bitmap *bmp, uint32_t x, uint32_t y, Color color)
{
	bmp->setPixel(x, y, color);
}

extern "C" DLL_REN_API void* Bitmap_GetTexture2D(Bitmap *bmp)
{
	return bmp->getTexture2D().as<void>();
}