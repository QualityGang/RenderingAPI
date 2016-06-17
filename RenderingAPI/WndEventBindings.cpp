#include "WndEvent.h"

#include "DLLExport.h"

extern "C" DLL_REN_API WndEvent* WndEvent_Create()
{
	return new WndEvent;
}

extern "C" DLL_REN_API void WndEvent_Release(WndEvent *e)
{
	delete e;
}

extern "C" DLL_REN_API bool WndEvent_IsQuitEvent(WndEvent *e)
{
	return e->type == WndEvent::Quit;
}
