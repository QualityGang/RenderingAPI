#pragma once

#ifndef STATIC_LIB
#ifdef DLL_EXPORT
#define DLL_REN_API  __declspec(dllexport)
#else
#define DLL_REN_API  __declspec(dllimport)
#endif
#endif
