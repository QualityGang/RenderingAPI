#include "DynLib.h"

#include <string>

#include "Throw.h"

DynLib::DynLib()
{
}

DynLib::~DynLib()
{
	unload();
}

void DynLib::load(const char *path)
{
	if (handle)
		return;

	std::string pathStr = path;

	if (pathStr.substr(pathStr.length() - 4, 4) != ".dll")
		pathStr += ".dll";

	handle = (HINSTANCE)LoadLibrary(pathStr.c_str());

	if (!handle)
		THROW("Failed to load dynamic library");
}

void DynLib::unload()
{
	if (!handle)
		return;

	if (!FreeLibrary(handle))
		THROW("Failed to unload dynamic library");
}

void* DynLib::getSymbol(const char *symbol) const
{
	if (!handle)
		return nullptr;

	return (void*)GetProcAddress(handle, symbol);
}
