#include "Sprite.h"

extern "C" DLL_REN_API Sprite* Sprite_Create()
{
	return new Sprite;
}

extern "C" DLL_REN_API void Sprite_Release(Sprite *sprite)
{
	delete sprite;
}

extern "C" DLL_REN_API void Sprite_SetPosition(Sprite *sprite, float x, float y)
{
	sprite->setPosition(x, y);
}

extern "C" DLL_REN_API void Sprite_SetDepth(Sprite *sprite, float depth)
{
	sprite->setDepth(depth);
}

extern "C" DLL_REN_API void Sprite_SetAngle(Sprite *sprite, float degrees)
{
	sprite->setAngle(degrees);
}

extern "C" DLL_REN_API void Sprite_SetSize(Sprite *sprite, float width, float height)
{
	sprite->setSize(width, height);
}

extern "C" DLL_REN_API void Sprite_SetColor(Sprite *sprite, Color color)
{
	sprite->setColor(color);
}

extern "C" DLL_REN_API void Sprite_SetSrcRect(Sprite *sprite, float srcX, float srcY, float srcWidth, float srcHeight)
{
	sprite->setSrcRect(FloatRect(srcX, srcY, srcWidth, srcHeight));
}

extern "C" DLL_REN_API void Sprite_SetTexture(Sprite *sprite, void *texture)
{
	sprite->setTexture(hTexture2D(texture));
}

extern "C" DLL_REN_API void Sprite_SetEffect(Sprite *sprite, SpriteEffect effect)
{
	sprite->setEffect(effect);
}

extern "C" DLL_REN_API float Sprite_GetX(Sprite *sprite)
{
	return sprite->getX();
}

extern "C" DLL_REN_API float Sprite_GetY(Sprite *sprite)
{
	return sprite->getY();
}

extern "C" DLL_REN_API float Sprite_GetDepth(Sprite *sprite)
{
	return sprite->getDepth();
}

extern "C" DLL_REN_API float Sprite_GetWidth(Sprite *sprite)
{
	return sprite->getWidth();
}

extern "C" DLL_REN_API float Sprite_GetHeight(Sprite *sprite)
{
	return sprite->getHeight();
}

extern "C" DLL_REN_API float Sprite_GetAngle(Sprite *sprite)
{
	return sprite->getAngle();
}

//extern "C" DLL_REN_API const Color<float> Sprite_GetColor()
//{
//
//}

//extern "C" DLL_REN_API hTexture2D Sprite_GetTexture(Sprite *sprite)
//{
//	return sprite->getTexture();
//}

//extern "C" DLL_REN_API const Rect<float> Sprite_GetSrcRect()
//{
//
//}

extern "C" DLL_REN_API SpriteEffect Sprite_GetEffect(Sprite *sprite)
{
	return sprite->getEffect();
}
