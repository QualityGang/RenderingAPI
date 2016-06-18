#pragma once

#include <Throw.h>

#define SafeRelease(ptr) do { if (ptr) ptr->Release(); ptr = nullptr; } while (false)

#if _DEBUG
#define INCONSISTENT_FLAGS(flags, a, b) do { if ((flags & a) && (flags & b)) { THROW("Inconsistent Flags"); } } while (false)
#else
#define INCONSISTENT_FLAGS(flags, a, b)
#endif
