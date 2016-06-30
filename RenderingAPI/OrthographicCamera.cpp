#include "OrthographicCamera.h"

using namespace DirectX;

OrthographicCamera::OrthographicCamera(float x, float y, float viewportWidth, float viewportHeight, bool update)
	: Camera(viewportWidth, viewportHeight), x(x), y(y)
{
	if (update)
		this->update();
}

OrthographicCamera::OrthographicCamera(float viewportWidth, float viewportHeight, bool update)
	: Camera(viewportWidth, viewportHeight)
{
	if (update)
		this->update();
}

OrthographicCamera::~OrthographicCamera()
{
}

void OrthographicCamera::update()
{
	float halfVpWidth  = viewportWidth  / 2.0f;
	float halfVpHeight = viewportHeight / 2.0f;

	float rad  = XMConvertToRadians(angle);
	float zoom = this->zoom < 0 ? 0 : this->zoom;

	projection = (viewportWidth == 0 || viewportHeight == 0) ?
		XMMatrixIdentity() : XMMatrixOrthographicOffCenterLH(zoom * -halfVpWidth, zoom * halfVpWidth, zoom * halfVpHeight, zoom * -halfVpHeight, 0.0f, 1.0f);

	view = XMMatrixTranslation(-x, -y, 0.0f) *
		   XMMatrixRotationZ(rad);

	combined    = view * projection;
	invCombined = XMMatrixInverse(nullptr, combined);
}

void OrthographicCamera::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void OrthographicCamera::setViewport(float viewportWidth, float viewportHeight)
{
	this->viewportWidth  = viewportWidth;
	this->viewportHeight = viewportHeight;
}

void OrthographicCamera::setAngle(float deg)
{
	this->angle = deg;
}

void OrthographicCamera::setZoom(float zoom)
{
	this->zoom = zoom;
}
