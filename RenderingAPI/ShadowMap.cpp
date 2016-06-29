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
{
}
#include <string>
void ShadowMap::update(const Window &window, const OrthographicCamera &camera)
{
	float left = 0;
	float top = 0;
	float right = window.getSize().x;
	float bottom = window.getSize().y;

	DirectX::XMFLOAT4 leftTop(left, top, 0.0f, 1.0f);
	camera.unproject(window, leftTop);

	left = leftTop.x;
	top = leftTop.y;

	DirectX::XMFLOAT4 rightBottom(right, bottom, 0.0f, 1.0f);
	camera.unproject(window, rightBottom);

	right = rightBottom.x;
	bottom = rightBottom.y;

	shadowSprite.setPosition(left, top);
	shadowSprite.setSize(right - left, bottom - top);
	//shadowSprite.setAngle(camera.getAngle());
}
