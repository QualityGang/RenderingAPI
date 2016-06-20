#pragma once

#include "Color.h"

class Text
{
public:
	Text()  = default;
	~Text() = default;

	void setString(const char *str)    { this->str     = str;      }
	void setPosition(float x, float y) { this->x       = x;
										 this->y       = y;		   }
	void setDepth(float depth)         { this ->depth  = depth;    }
	void setLineGap(float lineGap)     { this->lineGap = lineGap;  }
	void setColor(const Color &color)  { this->color   = color;    }

	const char* getString() const { return str;     }
	float getX()			const { return x;	    }
	float getY()			const { return y;       }
	float getDepth()		const { return depth;   }
	float getLineGap()		const { return lineGap; }
	const Color& getColor() const { return color;   }
private:
	const char *str = "";
	float x = 0, y = 0, depth = 0;
	float lineGap = 0;
	Color color;
};

