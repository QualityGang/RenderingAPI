#include "ShadowMap.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "FreeImage.h"

ShadowMap::ShadowMap(GraphicsContext *context, uint32_t width, uint32_t height) : context(context), renderTexture(context, width, height)
{
	context->clearRenderTarget(renderTexture.getRenderTarget(), 0, Color(0, 0, 0, 150));

	shadowSprite.setPosition(0, 0);
	shadowSprite.setSize(800, 600);
	shadowSprite.setSrcRect(FloatRect(0, 0, 800, 600));
	shadowSprite.setTexture(renderTexture.getTexture2D());
}

ShadowMap::~ShadowMap()
{}

void ShadowMap::Update(OrthographicCamera* camera)
{
	float width = camera->getViewportWidth() / camera->getZoom();
	float height = camera->getViewportHeight() / camera->getZoom();
	shadowSprite.setSize(width, height);
	shadowSprite.setPosition(camera->getX() + width / 2, camera->getY() + height / 2);
	shadowSprite.setAngle(camera->getAngle());
}

Sprite ShadowMap::getShadowSprite() const
{
	return shadowSprite;
}

RenderTexture ShadowMap::getRenderTexture() const
{
	return renderTexture;
}
