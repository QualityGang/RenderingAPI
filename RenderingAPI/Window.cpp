#include "Window.h"

#include "Throw.h"

#define IsMenuActiveByAlt(lParam) ((lParam >> 16) <= 0)

#define WND_CLASSNAME "WndClassName"

#define RID_USAGE_MOUSE 2
#define RID_USAGE_KEYBOARD 6

Window::StaticInit Window::__static_init;

uint32_t Window::windowCount = 0;

Window::Window(GraphicsContext *context, const char *title, int width, int height)
	: context(context)
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);

	int clientWidth  = width;
	int clientHeight = height;

	clientToScreen(&width, &height, WS_OVERLAPPEDWINDOW);

	hwnd = CreateWindow(WND_CLASSNAME, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
		THROW("Failed to create hwnd");

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

	int w = 0;
	int h = 0;
	clientToScreen(&w, &h, WS_OVERLAPPEDWINDOW);
	minWndSize.x = w;
	minWndSize.y = h;

	ShowWindow(hwnd, SW_SHOW);
	windowCount++;

	renderWindow = context->createRenderWindow(hwnd, clientWidth, clientHeight, 2, PixelFormat_RGBA8, 1, true);
}

Window::~Window()
{
	close();
}

Window::StaticInit::StaticInit()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = (HINSTANCE)GetModuleHandle(nullptr);
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszClassName = WND_CLASSNAME;
	//wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wcex))
		THROW("Failed to register wnd class");
}

Window::StaticInit::~StaticInit()
{
}

void Window::setTitle(const char *title)
{
	SetWindowText(hwnd, title);
}

void Window::setPosition(int x, int y)
{
	SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::setSize(int width, int height)
{
	clientToScreen(&width, &height);
	SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::setMinSize(int width, int height)
{
	minSize.x = width;
	minSize.y = height;

	clientToScreen(&width, &height);

	minWndSize.x = width;
	minWndSize.y = height;
}

void Window::setResizable(bool resizable)
{
	LONG style = GetWindowLong(hwnd, GWL_STYLE);

	if (resizable)
		style |= WS_THICKFRAME;
	else
		style &= ~WS_THICKFRAME;

	SetWindowLong(hwnd, GWL_STYLE, style);
}

void Window::setMaximizable(bool maximizable)
{
	LONG style = GetWindowLong(hwnd, GWL_STYLE);

	if (maximizable)
		style |= WS_MAXIMIZEBOX;
	else
		style &= ~WS_MAXIMIZEBOX;

	SetWindowLong(hwnd, GWL_STYLE, style);
}

void Window::setIcon(const Bitmap &bmp)
{
	DestroyIcon(icon);

	uint32_t size = bmp.getWidth() * bmp.getHeight() * 4;

	BYTE *buffer = new BYTE[size];

	for (uint32_t i = 0; i < bmp.getWidth() * bmp.getHeight(); i++)
	{
		Color pixel;
		bmp.getPixel(i, &pixel);

		// RGBA to BGRA
		BYTE *bufferIndex = &buffer[i * 4];
		*bufferIndex++ = pixel.b;
		*bufferIndex++ = pixel.g;
		*bufferIndex++ = pixel.r;
		*bufferIndex   = pixel.a;
	}

	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);
	this->icon = CreateIcon(hInstance, bmp.getWidth(), bmp.getHeight(), 1, 32, nullptr, buffer);

	delete[] buffer;

	if (!icon)
		THROW("Failed to create icon");

	SendMessage(hwnd, WM_SETICON, ICON_BIG,   (LPARAM)icon);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
}

void Window::setVSyncEnabled(bool vsync)
{
	context->setVSyncEnabled(renderWindow, vsync);
}

const char* Window::getTitle() const
{
	int len = GetWindowTextLength(hwnd) + 1;
	char *buf = new char[len];
	GetWindowText(hwnd, buf, len);
	return buf;
}

POINT Window::getPosition() const
{
	RECT wr;
	GetWindowRect(hwnd, &wr);
	POINT p = { wr.left, wr.top };
	return p;
}

POINT Window::getMousePosition() const
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);
	return p;
}

POINT Window::getSize() const
{
	RECT cr;
	GetClientRect(hwnd, &cr);

	POINT p = { cr.right - cr.left, cr.bottom - cr.top };
	return p;
}

const POINT& Window::getMinSize() const
{
	return minSize;
}

HWND Window::getHandle() const
{
	return hwnd;
}

hRenderTarget Window::getRenderTarget() const
{
	return context->getRenderTarget(renderWindow);
}

bool Window::isKeyPressed(Key key) const
{
	return keys[key];
}

bool Window::isResizable() const
{
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	return (style & WS_THICKFRAME) == WS_THICKFRAME;
}

bool Window::isMaximizable() const
{
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	return (style & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX;
}

bool Window::isMinimized() const
{
	return IsIconic(hwnd) > 0;
}

bool Window::isInWindow(int x, int y, bool inClientSpace) const
{
	RECT rect;

	if (inClientSpace)
		GetClientRect(hwnd, &rect);
	else
		GetWindowRect(hwnd, &rect);

	POINT cursorPos = { x, y };
	return PtInRect(&rect, cursorPos) > 0;
}

bool Window::isVSyncEnabled() const
{
	return context->isVSyncEnabled(renderWindow);
}

bool Window::getNextEvent(WndEvent *e) const
{
	if (PeekMessage(&e->msg, hwnd, 0, 0, PM_REMOVE))
	{
		if (e->msg.message == WM_QUIT)
		{
			e->type = WndEvent::Quit;
			e->quit.exitCode = (int)e->msg.wParam;
		}
		else
		{
			e->type = WndEvent::Unknown;
		}
		
		return true;
	}

	return false;
}

void Window::processEvent(const WndEvent &e) const
{
	DispatchMessage(&e.msg);
}

void Window::close()
{
	SendMessage(hwnd, WM_CLOSE, (WPARAM)0, (LPARAM)0);
	context->releaseRenderWindow(renderWindow);
	DestroyIcon(icon);
}

void Window::clear(const Color &color)
{
	hRenderTarget renderTarget = context->getRenderTarget(renderWindow);

	context->clearRenderTarget(renderTarget, 0, color);
	context->clearDepthStencil(renderTarget, 1.0f, 0);
}

void Window::swapBuffers()
{
	context->swapBuffers(renderWindow);
}

void Window::clientToScreen(int *width, int *height) const
{
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	clientToScreen(width, height, style);
}

void Window::clientToScreen(int *width, int *height, LONG style) const
{
	RECT wr = { 0, 0, *width, *height };
	if (!AdjustWindowRect(&wr, style, FALSE))
		THROW("Failed to adjust wnd rect");

	*width  = wr.right  - wr.left;
	*height = wr.bottom - wr.top;
}

void Window::setKeyState(Key key, bool pressed)
{
	keys[key] = pressed;
}

void Window::setMouseKeyState(RAWINPUT *ri, USHORT buttonFlagDown, USHORT buttonFlagUp, Key key)
{
	POINT p = getMousePosition();

	if (isInWindow(p.x, p.y, true) && (ri->data.mouse.usButtonFlags & buttonFlagDown))
		setKeyState(key, true);
	else if (ri->data.mouse.usButtonFlags & buttonFlagUp)
		setKeyState(key, false);
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window *window = GetWindow(hwnd);

	switch (msg)
	{
		case WM_CREATE:
		{
			const int size = 2;

			RAWINPUTDEVICE rid[size];

			rid[0].usUsagePage = 1;
			rid[0].usUsage = RID_USAGE_MOUSE;
			rid[0].dwFlags = 0;
			rid[0].hwndTarget = hwnd;

			rid[1].usUsagePage = 1;
			rid[1].usUsage = RID_USAGE_KEYBOARD;
			rid[1].dwFlags = 0;
			rid[1].hwndTarget = hwnd;

			RegisterRawInputDevices(rid, size, sizeof(RAWINPUTDEVICE));
		} break;

		case WM_INPUT:
		{
			UINT dwSize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

			BYTE *buffer = new BYTE[dwSize];

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)buffer, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			{
				delete buffer;
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}

			RAWINPUT *raw = (RAWINPUT*)buffer;

			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				window->setMouseKeyState(raw, RI_MOUSE_LEFT_BUTTON_DOWN,   RI_MOUSE_LEFT_BUTTON_UP  , Key::LButton);
				window->setMouseKeyState(raw, RI_MOUSE_RIGHT_BUTTON_DOWN,  RI_MOUSE_RIGHT_BUTTON_UP,  Key::RButton);
				window->setMouseKeyState(raw, RI_MOUSE_MIDDLE_BUTTON_DOWN, RI_MOUSE_MIDDLE_BUTTON_UP, Key::MButton);
				window->setMouseKeyState(raw, RI_MOUSE_BUTTON_4_DOWN,      RI_MOUSE_BUTTON_4_UP,      Key::XButton1);
				window->setMouseKeyState(raw, RI_MOUSE_BUTTON_5_DOWN,      RI_MOUSE_BUTTON_5_UP,      Key::XButton2);

				// TODO: Mouse Wheel
				//short mouseZ = (short)raw->data.mouse.usButtonData;
			}
			else if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				USHORT keyCode = raw->data.keyboard.VKey;
				bool pressed = !(raw->data.keyboard.Flags & RI_KEY_BREAK);
				window->setKeyState((Key)keyCode, pressed);
			}

			delete buffer;
		} break;

		case WM_GETMINMAXINFO:
		{
			if (!window)
				return DefWindowProc(hwnd, msg, wParam, lParam);

			LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
			mmi->ptMinTrackSize.x = window->minWndSize.x;
			mmi->ptMinTrackSize.y = window->minWndSize.y;
		} break;

		case WM_SIZE:
		{
			int width  = (int)(short)LOWORD(lParam);
			int height = (int)(short)HIWORD(lParam);

			if (window && window->renderWindow && width > 0 && height > 0)
				window->context->resize(window->renderWindow, width, height);
		} break;

		case WM_DESTROY:
			window->hwnd = nullptr;

			if (--windowCount == 0)
				PostQuitMessage(EXIT_SUCCESS);

			break;

		case WM_SYSCOMMAND:
			switch (wParam)
			{
				case SC_KEYMENU:
					if (IsMenuActiveByAlt(lParam))
						return 0;
					break;
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

Window* Window::GetWindow(HWND hwnd)
{
	return (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}