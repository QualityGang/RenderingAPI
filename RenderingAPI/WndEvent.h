#pragma once

#include <Windows.h>
#include <stdint.h>

struct WndEvent
{
	struct QuitEvent
	{
		int exitCode;
	};

	struct MouseWheelEvent
	{
		int32_t delta;
	};

	enum EventType
	{
		Unknown,
		Quit,
		MouseWheel
	};

	EventType type = Unknown;

	union
	{
		QuitEvent       quit;
		MouseWheelEvent mouseWheel;
	};

	MSG msg = { 0 };
};
