#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::setPosition(float x, float y)
{
	this->dst.x = x;
	this->dst.y = y;
}

void Sprite::setDepth(float depth)
{
	this->depth = depth;
}

void Sprite::setAngle(float degrees)
{
	this->degrees = degrees;
}

void Sprite::setSize(float width, float height)
{
	this->dst.width = width;
	this->dst.height = height;
}

void Sprite::setColor(const Color &color)
{
	this->color = color;
}

void Sprite::setSrcRect(const FloatRect &src)
{
	this->src = src;
}

void Sprite::setTexture(hTexture2D texture)
{
	this->texture = texture;
}

void Sprite::setEffect(SpriteEffect effect)
{
	this->effect = effect;
}

void Sprite::setPositionAnchor(float anchorX, float anchorY)
{
	this->posAnchor.x = anchorX;
	this->posAnchor.y = anchorY;
}

void Sprite::setRotationAnchor(float anchorX, float anchorY)
{
	this->rotAnchor.x = anchorX;
	this->rotAnchor.y = anchorY;
}

float Sprite::getX() const
{
	return dst.x;
}

float Sprite::getY() const
{
	return dst.y;
}

float Sprite::getDepth() const
{
	return depth;
}

float Sprite::getWidth() const
{
	return dst.width;
}

float Sprite::getHeight() const
{
	return dst.height;
}

float Sprite::getAngle() const
{
	return degrees;
}

const Color& Sprite::getColor() const
{
	return color;
}

hTexture2D Sprite::getTexture() const
{
	return texture;
}

const FloatRect Sprite::getSrcRect() const
{
	return src;
}

SpriteEffect Sprite::getEffect() const
{
	return effect;
}

float Sprite::getPositionAnchorX() const
{
	return posAnchor.x;
}

float Sprite::getPositionAnchorY() const
{
	return posAnchor.y;
}

float Sprite::getRotationAnchorX() const
{
	return rotAnchor.x;
}

float Sprite::getRotationAnchorY() const
{
	return rotAnchor.y;
}
