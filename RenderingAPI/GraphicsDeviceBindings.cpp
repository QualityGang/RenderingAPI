#include "GraphicsDevice.h"

extern "C" DLL_REN_API GraphicsDevice* GraphicsDevice_Create(RenderingAPI api)
{
	return new GraphicsDevice(api);
}

extern "C" DLL_REN_API void GraphicsDevice_Release(GraphicsDevice *device)
{
	delete device;
}

extern "C" DLL_REN_API GraphicsContext* GraphicsDevice_GetContext(GraphicsDevice *device)
{
	return device->getContext();
}

extern "C" DLL_REN_API RenderingAPI GetAPI(GraphicsDevice *device)
{
	return device->getAPI();
}