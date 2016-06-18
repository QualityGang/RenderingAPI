#pragma once

#include <stdint.h>

struct Color
{
	Color() : r(255), g(255), b(255), a(255) {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r(r), g(g), b(b), a(a) {}
	~Color() {}

	void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	uint8_t r, g, b, a;
};
