#pragma once

#include <Windows.h>

#include "GraphicsContext.h"
#include "WndEvent.h"
#include "Bitmap.h"
#include "Keys.h"
#include "NonCopyable.h"

class Window : public NonCopyable
{
public:
	DLL_REN_API Window(GraphicsContext *context, const char *title, int width, int height);
	DLL_REN_API ~Window();
	 
	DLL_REN_API void setTitle(const char *title);
	DLL_REN_API void setPosition(int x, int y);
	DLL_REN_API void setSize(int width, int height);
	DLL_REN_API void setMinSize(int width, int height);
	DLL_REN_API void setResizable(bool resizable);
	DLL_REN_API void setMaximizable(bool maximizable);
	DLL_REN_API void setIcon(const Bitmap &bmp);
	DLL_REN_API void setVSyncEnabled(bool vsync);
	 
	DLL_REN_API const char* getTitle() const;
	DLL_REN_API POINT getPosition() const;
	DLL_REN_API POINT getMousePosition() const;
	DLL_REN_API POINT getSize() const;
	DLL_REN_API const POINT& getMinSize() const;
	DLL_REN_API HWND getHandle() const;
	DLL_REN_API hRenderTarget getRenderTarget() const;
	 
	DLL_REN_API bool isKeyPressed(Key key) const;
	DLL_REN_API bool isResizable() const;
	DLL_REN_API bool isMaximizable() const;
	DLL_REN_API bool isMinimized() const;
	DLL_REN_API bool isInWindow(int x, int y, bool inClientSpace) const;
	DLL_REN_API bool isVSyncEnabled() const;
	 
	DLL_REN_API bool getNextEvent(WndEvent *e) const;
	DLL_REN_API void processEvent(const WndEvent &e) const;
	 
	DLL_REN_API void close();
	
	DLL_REN_API void clear(const Color &color);
	DLL_REN_API void swapBuffers();
	 
	DLL_REN_API static Window* GetWindow(HWND hwnd);
private:
	HWND hwnd;
	HICON icon = nullptr;
	POINT minSize = { 0 }, minWndSize = { 0 };
	bool keys[NUM_KEYCODES] = { 0 };

	GraphicsContext *context;
	hRenderWindow renderWindow;

	static uint32_t windowCount;

	void clientToScreen(int *width, int *height) const;
	void clientToScreen(int *width, int *height, LONG style) const;

	void setKeyState(Key key, bool pressed);
	void setMouseKeyState(RAWINPUT *ri, USHORT buttonFlagDown, USHORT buttonFlagUp, Key key);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	class StaticInit
	{
	public:
		StaticInit();
		virtual ~StaticInit();
	};

	static StaticInit __static_init;
};

