#pragma once

#include "GraphicsContext.h"
#include "Rect.h"
#include "SpriteEffect.h"

class Sprite
{
public:
	DLL_REN_API Sprite();
	DLL_REN_API ~Sprite();
	 
	DLL_REN_API void setPosition(float x, float y);
	DLL_REN_API void setDepth(float depth);
	DLL_REN_API void setAngle(float degrees);
	DLL_REN_API void setSize(float width, float height);
	DLL_REN_API void setColor(const Color &color);
	DLL_REN_API void setSrcRect(const FloatRect &src);
	DLL_REN_API void setTexture(hTexture2D texture);
	DLL_REN_API void setEffect(SpriteEffect effect);
	 
	DLL_REN_API float getX() const;
	DLL_REN_API float getY() const;
	DLL_REN_API float getDepth() const;
	DLL_REN_API float getWidth() const;
	DLL_REN_API float getHeight() const;
	DLL_REN_API float getAngle() const;
	DLL_REN_API const Color& getColor() const;
	DLL_REN_API hTexture2D getTexture() const;
	DLL_REN_API const FloatRect getSrcRect() const;
	DLL_REN_API SpriteEffect getEffect() const;
private:
	FloatRect src;
	FloatRect dst;
	float degrees       = 0;
	float depth         = 0;
	SpriteEffect effect = SpriteEffect_None;
	Color color;
	hTexture2D texture  = nullptr;
};