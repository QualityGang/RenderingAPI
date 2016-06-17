#pragma once

#include <exception>
#include <string>

#define THROW(s) \
	do \
	{  \
		OutputDebugString(s); \
		OutputDebugString(" " __FILE__ " "); \
		OutputDebugString(std::to_string(__LINE__).c_str()); \
		OutputDebugString("\n"); \
		throw std::exception(s);  \
	} while (false)

