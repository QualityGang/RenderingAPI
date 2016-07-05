#include "DebugConsoleCommand.h"

#include "DebugConsole.h"

DebugConsoleCommand::DebugConsoleCommand(void *value, const char *name, Type type)
	: value(value), name(name), type(type)
{
	DebugConsole::AddCommand(*this);
}

DebugConsoleCommand::DebugConsoleCommand(int32_t &value, const char *name)
	: DebugConsoleCommand((void*)&value, name, Type_Int32)
{
}

DebugConsoleCommand::DebugConsoleCommand(uint32_t &value, const char *name)
	: DebugConsoleCommand((void*)&value, name, Type_UInt32)
{
}

DebugConsoleCommand::DebugConsoleCommand(float &value, const char *name)
	: DebugConsoleCommand((void*)&value, name, Type_Float)
{
}

DebugConsoleCommand::DebugConsoleCommand(double &value, const char *name)
	: DebugConsoleCommand((void*)&value, name, Type_Double)
{
}

DebugConsoleCommand::DebugConsoleCommand(char *value, const char *name)
	: DebugConsoleCommand((void*)value, name, Type_String)
{
}

DebugConsoleCommand::DebugConsoleCommand(CallbackFunction value, const char *name)
	: DebugConsoleCommand((void*)value, name, Type_Function)
{
}
