#include "Entity.h"


Entity::Entity(GraphicsContext *context)
	: context(context)
{
}

Entity::~Entity()
{
}

void Entity::getSize(Vec2 *vec2) const
{
	TextureSize size;
	context->getTexture2DSize(image, &size);

	vec2->x = (float)size.width;
	vec2->y = (float)size.height;
}

void Entity::draw(SpriteBatch &spriteBatch)
{
	Vec2 size;
	getSize(&size);

	Sprite sprite;
	sprite.setPositionAnchor(0.5f, 0.5f);
	sprite.setPosition(position.x, position.y);
	sprite.setSize(size.x, size.y);
	sprite.setSrcRect(FloatRect(0.0f, 0.0f, size.x, size.y));
	sprite.setAngle(orientation);
	sprite.setColor(color);
	sprite.setTexture(image);

	spriteBatch.draw(sprite);
}