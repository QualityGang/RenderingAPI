#include "Bitmap.h"

#include <FreeImage.h>
#include <xutility>

#include "Throw.h"

Bitmap::Bitmap(GraphicsContext *context)
	: context(context)
{
}

Bitmap::Bitmap(GraphicsContext *context, uint32_t width, uint32_t height, const uint8_t *pixels)
	: context(context)
{
	create(width, height, pixels);
}

Bitmap::Bitmap(GraphicsContext *context, const char *fileName)
	: context(context)
{
	create(fileName);
}

Bitmap::~Bitmap()
{
	delete[] buffer;
	context->releaseTexture2D(texture);
}

Bitmap::Bitmap(const Bitmap &other)
{
	width   = other.width;
	height  = other.height;
	texture = nullptr;

	uint32_t size = width * height * 4;
	buffer = new uint8_t[size];
	memcpy(buffer, other.buffer, size);
}

Bitmap::Bitmap(Bitmap &&other)
{
	buffer  = other.buffer;
	width   = other.width;
	height  = other.height;
	texture = other.texture;

	other.buffer  = nullptr;
	other.width   = 0;
	other.height  = 0;
	other.texture = nullptr;
}

Bitmap& Bitmap::operator=(const Bitmap &other)
{
	if (this != &other)
		*this = std::move(Bitmap(other));

	return *this;
}

Bitmap& Bitmap::operator=(Bitmap &&other)
{
	if (this != &other)
	{
		delete[] buffer;
		context->releaseTexture2D(texture);

		buffer  = other.buffer;
		width   = other.width;
		height  = other.height;
		dirty   = other.dirty;
		texture = other.texture;

		other.buffer  = nullptr;
		other.width   = 0;
		other.height  = 0;
		other.texture = nullptr;;
	}

	return *this;
}

void Bitmap::create(const char *fileName)
{
	static struct _StaticInit
	{
		_StaticInit::_StaticInit()
		{
			FreeImage_Initialise();
		}

		_StaticInit::~_StaticInit()
		{
			FreeImage_DeInitialise();
		}
	} __static_init;

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName);

	if (format == FIF_UNKNOWN)
		THROW("Unknown format");

	FIBITMAP *image = nullptr;

	if (FreeImage_FIFSupportsReading(format))
		image = FreeImage_Load(format, fileName);

	if (!image)
		THROW("Failed to load image");

	if (!FreeImage_FlipVertical(image))
		THROW("Failed to flip image vertically");

	FIBITMAP *tmp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(tmp);

	if (!image)
		THROW("Failed to convert image to 32 bits");

	width  = FreeImage_GetWidth(image);
	height = FreeImage_GetHeight(image);
	uint32_t size = width * height * 4;

	buffer = new uint8_t[size];

	BYTE *pixels = FreeImage_GetBits(image);

	// BGRA to RGBA
	for (uint32_t i = 0; i < width * height; i++)
	{
		Color color(
			pixels[i * 4 + 2],
			pixels[i * 4 + 1],
			pixels[i * 4 + 0],
			pixels[i * 4 + 3]
		);

		setPixel(i, color);
	}

	FreeImage_Unload(image);
}

void Bitmap::create(uint32_t width, uint32_t height, const uint8_t *pixels)
{
	this->width  = width;
	this->height = height;

	uint32_t size = width * height * 4;
	buffer = new uint8_t[size];

	if (pixels)
		memcpy(buffer, pixels, size);
	else
		memset(buffer, 0, size);
}

void Bitmap::setPixel(uint32_t index, const Color &color)
{
	uint8_t *pixel = &buffer[index * 4];
	*pixel++ = color.r;
	*pixel++ = color.g;
	*pixel++ = color.b;
	*pixel   = color.a;

	dirty = true;
}

void Bitmap::setPixel(uint32_t x, uint32_t y, const Color &color)
{
	setPixel(y * width + x, color);
}

void Bitmap::getPixel(uint32_t index, Color *color) const
{
	uint8_t *pixel = &buffer[index * 4];
	color->r = *pixel++;
	color->g = *pixel++;
	color->b = *pixel++;
	color->a = *pixel;
}

void Bitmap::getPixel(uint32_t x, uint32_t y, Color *color) const
{
	getPixel(y * width + x, color);
}

uint32_t Bitmap::getWidth() const
{
	return width;
}

uint32_t Bitmap::getHeight() const
{
	return height;
}

hTexture2D Bitmap::getTexture2D() const
{
	if (dirty)
	{
		updateTexture();
		dirty = false;
	}

	return texture;
}

void Bitmap::updateTexture() const
{
	if (!buffer)
		THROW("Invalid bmp buffer");

	if (width == 0 || height == 0)
		THROW("Invalid bmp size");

	if (texture)
	{
		MapData mapData;
		context->mapTexture2D(texture, MapType_Write, &mapData);
		uint8_t *texBuff = (uint8_t*)mapData.mem;

		uint8_t *buffer = this->buffer;

		for (uint8_t i = 0; i < height; i++)
		{
			memcpy(texBuff, buffer, width * 4);
			texBuff += mapData.rowPitch;
			buffer += width * 4;
		}

		context->unmapTexture2D(texture);
	}
	else
	{
		texture = context->createTexture2D(width, height, PixelFormat_RGBA8, 1, 1,
			TextureFlag_ShaderResource, AccessFlag_Write, buffer, width * 4);
	}

}