#include "Bitmap.h"

#include <FreeImage.h>
#include <xutility>

#include "Throw.h"

#include <string>

Bitmap::_StaticInit Bitmap::__static_init;

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

Bitmap::Bitmap(GraphicsContext *context, hTexture2D texture)
	: context(context)
{
	create(texture);
}

Bitmap::~Bitmap()
{
	OutputDebugString(std::to_string((int64_t)context).c_str());
	OutputDebugString("\n");

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

Bitmap::_StaticInit::_StaticInit()
{
	FreeImage_Initialise();
}

Bitmap::_StaticInit::~_StaticInit()
{
	FreeImage_DeInitialise();
}

void Bitmap::create(const char *fileName)
{
	this->~Bitmap();

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
	this->~Bitmap();

	this->width  = width;
	this->height = height;

	uint32_t size = width * height * 4;
	buffer = new uint8_t[size];

	if (pixels)
		memcpy(buffer, pixels, size);
	else
		memset(buffer, 0, size);
}

void Bitmap::create(hTexture2D texture)
{
	this->~Bitmap();

	TextureSize texSize;
	context->getTexture2DSize(texture, &texSize);

	this->width  = texSize.width;
	this->height = texSize.height;

	uint32_t size = width * height * 4;
	buffer = new uint8_t[size];

	hTexture2D tmp = context->createTexture2D(width, height, PixelFormat_RGBA8, 1, 1,
		TextureFlag_None, AccessFlag_Read | AccessFlag_Write, nullptr, 0);

	context->copyTexture2D(texture, tmp);

	MapData mapData;
	context->mapTexture2D(tmp, MapType_Read, &mapData);

	uint8_t *data   = (uint8_t*)mapData.mem;
	uint8_t *texBuf = this->buffer;

	for (uint32_t i = 0; i < height; i++)
	{
		memcpy(texBuf, data, width * 4);
		data += mapData.rowPitch;
		texBuf += width * 4;
	}

	context->unmapTexture2D(tmp);
	context->releaseTexture2D(tmp);
}

bool Bitmap::save(const char *fileName) const
{
	std::string fileNameStr = fileName;

	if (fileNameStr.substr(fileNameStr.length() - 4, 4) != ".png")
		fileNameStr += ".png";

	uint32_t size = width * height * 4;
	uint8_t *bgraBuffer = new uint8_t[size];

	// RGBA to BGRA
	for (uint32_t i = 0; i < width * height * 4; i += 4)
	{
		bgraBuffer[i + 0] = buffer[i + 2];
		bgraBuffer[i + 1] = buffer[i + 1];
		bgraBuffer[i + 2] = buffer[i + 0];
		bgraBuffer[i + 3] = buffer[i + 3];
	}

	FIBITMAP *image = FreeImage_ConvertFromRawBits(bgraBuffer, width, height, width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);
	bool result = FreeImage_Save(FIF_PNG, image, fileNameStr.c_str(), 0) > 0;
	FreeImage_Unload(image);
	delete[] bgraBuffer;
	return result;
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