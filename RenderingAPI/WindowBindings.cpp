#include "Window.h"

#include "GraphicsContext.h"

extern "C" DLL_REN_API Window* Window_Create(GraphicsContext *context, const char *title, int width, int height)
{
	return new Window(context, title, width, height);
}

extern "C" DLL_REN_API void Window_Release(Window *window)
{
	delete window;
}

extern "C" DLL_REN_API void Window_Clear(Window *window, Color color)
{
	window->clear(color);
}

extern "C" DLL_REN_API void Window_SwapBuffers(Window *window)
{
	window->swapBuffers();
}

extern "C" DLL_REN_API bool Window_GetNextEvent(Window *window, WndEvent *e)
{
	return window->getNextEvent(e);
}

extern "C" DLL_REN_API void Window_ProcessEvent(Window *window, WndEvent *e)
{
	window->processEvent(*e);
}

extern "C" DLL_REN_API void* Window_GetRenderTarget(Window *window)
{
	return window->getRenderTarget().as<void>();
}