#pragma once

#include <Windows.h>

#include "NonCopyable.h"
#include "DLLExport.h"

class DynLib : public NonCopyable
{
public:
	DLL_REN_API DynLib();
	DLL_REN_API ~DynLib();
	 
	DLL_REN_API void load(const char *path);
	DLL_REN_API void unload();
	 
	DLL_REN_API void* getSymbol(const char *symbol) const;
private:
	HINSTANCE handle = nullptr;
};

