#pragma once

#include <SpriteBatch.h>
#include <Color.h>
#include <NonCopyable.h>

#include "Vec.h"

class Entity : public NonCopyable
{
public:
	Entity(GraphicsContext *context);
	~Entity();

	Vec2 position = Vec2{ 0.0f, 0.0f }, velocity = Vec2{ 0.0f, 0.0f };
	float orientation = 0.0f;
	float radius = 20.0f;
	bool isExpired = false;

	void getSize(Vec2 *vec2) const;

	virtual void update() = 0;
	virtual void draw(SpriteBatch &spriteBatch);
protected:
	hTexture2D image;
	Color color;
private:
	GraphicsContext *context;
};

