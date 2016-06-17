#pragma once

#include "GraphicsContext.h"
#include "RenderingAPI.h"
#include "DynLib.h"
#include "DLLExport.h"

class GraphicsDevice : public NonCopyable
{
public:
	DLL_REN_API explicit GraphicsDevice(RenderingAPI api);
	DLL_REN_API ~GraphicsDevice();

	RenderingAPI getAPI() const { return api; }
	GraphicsContext* getContext() const { return context; }
private:
	RenderingAPI api;
	GraphicsContext *context;
	DynLib lib;
};

