#pragma once

#ifndef STATIC_LIB
#ifdef DLL_D3D11_EXPORT
#define DLL_D3D11_API  __declspec(dllexport)
#else
#define DLL_D3D11_API  __declspec(dllimport)
#endif
#else
#define DLL_D3D11_API
#endif