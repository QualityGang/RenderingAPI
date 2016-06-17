#include "Bitmap.h"

extern "C" DLL_REN_API Bitmap* Bitmap_Create(GraphicsContext *context, const char *fileName)
{
	return new Bitmap(context, fileName);
}

extern "C" DLL_REN_API void Bitmap_Release(Bitmap *bmp)
{
	delete bmp;
}

extern "C" DLL_REN_API void* Bitmap_GetTexture2D(Bitmap *bmp)
{
	return bmp->getTexture2D().as<void>();
}