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
	XMMATRIX projection = (viewportWidth == 0 || viewportHeight == 0) ?
		XMMatrixIdentity() : XMMatrixOrthographicOffCenterLH(0.0f, viewportWidth, viewportHeight, 0.0f, 0.0f, 1.0f);

	float halfVpWidth  = viewportWidth  / 2.0f;
	float halfVpHeight = viewportHeight / 2.0f;

	float x = this->x + halfVpWidth;
	float y = this->y + halfVpHeight;

	float rad  = XMConvertToRadians(angle);
	float zoom = this->zoom < 0 ? 0 : this->zoom;

	XMMATRIX view = XMMatrixTranslation(-halfVpWidth, -halfVpHeight, 0.0f) *
					XMMatrixScaling(zoom, zoom, 1.0f) *
					XMMatrixRotationZ(rad) *
					XMMatrixTranslation(-x + halfVpWidth, -y + halfVpHeight, 0.0f);

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
