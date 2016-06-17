#pragma once

#include <vcruntime_exception.h>
#include <malloc.h>

#define ALIGN(a) __declspec(align(a))

template <size_t A>
class AlignedNew
{
public:
	void* operator new(size_t size);
	void operator delete(void* ptr);
};

template <size_t A>
void* AlignedNew<A>::operator new(size_t size)
{
	void* ptr = _aligned_malloc(size, A);

	if (!ptr)
	{
		throw std::bad_alloc();
	}

	return ptr;
}

template <size_t A>
void AlignedNew<A>::operator delete(void* ptr)
{
	_aligned_free(ptr);
}
