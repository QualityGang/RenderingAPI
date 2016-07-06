#pragma once

#include <stdint.h>

#include "DLLExport.h"

class DebugConsoleCommand
{
public:
	enum Type
	{
		Type_Unknown,
		Type_Bool,
		Type_Int32,
		Type_UInt32,
		Type_Float,
		Type_Double,
		Type_String,
		Type_Function
	};

	typedef bool(*CallbackFunction)(const char*);

	DLL_REN_API DebugConsoleCommand(void            *value, const char *name, Type type);
	DLL_REN_API DebugConsoleCommand(int32_t         &value, const char *name);
	DLL_REN_API DebugConsoleCommand(uint32_t        &value, const char *name);
	DLL_REN_API DebugConsoleCommand(float           &value, const char *name);
	DLL_REN_API DebugConsoleCommand(double          &value, const char *name);
	DLL_REN_API DebugConsoleCommand(char            *value, const char *name);
	DLL_REN_API DebugConsoleCommand(CallbackFunction value, const char *name);

	const char *getName() const { return name; }
	void* getValuePtr() const { return value; }
	Type getType() const { return type; }
private:
	const char *name;
	void *value;
	Type type;
};
