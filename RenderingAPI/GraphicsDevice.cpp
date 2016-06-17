#include "GraphicsDevice.h"

#include "Throw.h"

GraphicsDevice::GraphicsDevice(RenderingAPI api)
	: api(api)
{
	switch (api)
	{
		case RenderingAPI_Direct3D11:
			lib.load("Direct3D11.dll");
			break;

		default:
			THROW("Unknown rendering API");
	}

	typedef GraphicsContext* (*CreateContextFunc)();
	CreateContextFunc createContextFunc = (CreateContextFunc)lib.getSymbol("CreateContext");

	context = createContextFunc();
}

GraphicsDevice::~GraphicsDevice()
{
	typedef void (*FreeContextFunc)(GraphicsContext *context);
	FreeContextFunc freeContextFunc = (FreeContextFunc)lib.getSymbol("FreeContext");

	freeContextFunc(context);
}
