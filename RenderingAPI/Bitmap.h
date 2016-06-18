#pragma once

#include "GraphicsContext.h"

class Bitmap
{
public:
	DLL_REN_API explicit Bitmap(GraphicsContext *context);
	DLL_REN_API Bitmap(GraphicsContext *context, const char *fileName);
	DLL_REN_API Bitmap(GraphicsContext *context, uint32_t width, uint32_t height, const uint8_t *pixels = nullptr);
	DLL_REN_API ~Bitmap();
	
	DLL_REN_API Bitmap(const Bitmap &other);
	DLL_REN_API Bitmap(Bitmap &&other);
	
	DLL_REN_API Bitmap& operator=(const Bitmap &other);
	DLL_REN_API Bitmap& operator=(Bitmap &&other);

	DLL_REN_API void create(const char *fileName);
	DLL_REN_API void create(uint32_t width, uint32_t height, const uint8_t *pixels = nullptr);
	
	DLL_REN_API void setPixel(uint32_t index, const Color &color);
	DLL_REN_API void setPixel(uint32_t x, uint32_t y, const Color &color);
	
	DLL_REN_API void getPixel(uint32_t index, Color *color) const;
	DLL_REN_API void getPixel(uint32_t x, uint32_t y, Color *color) const;
	
	DLL_REN_API uint32_t getWidth() const;
	DLL_REN_API uint32_t getHeight() const;
	DLL_REN_API hTexture2D getTexture2D() const;
private:
	void updateTexture() const;

	GraphicsContext *context;
	uint32_t width, height;
	uint8_t *buffer = nullptr;

	mutable bool dirty = true;

	mutable hTexture2D texture;
};

