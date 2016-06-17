#pragma once

template <typename T1, typename T2 = T1>
struct TRect
{
	TRect() : x(0), y(0), width(0), height(0) {}
	TRect(T1 x, T1 y, T2 width, T2 height)
		: x(x), y(y), width(width), height(height) {}
	~TRect() {}

	void set(T1 x, T1 y, T2 width, T2 height)
	{
		this->x      = x;
		this->y      = y;
		this->width  = width;
		this->height = height;
	}

	T1 x, y;
	T2 width, height;
};

typedef TRect<int32_t>  IntRect;
typedef TRect<uint32_t> UIntRect;
typedef TRect<float>	FloatRect;