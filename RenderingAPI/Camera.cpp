#include "Camera.h"

#include "Window.h"

using namespace DirectX;

Camera::Camera(float viewportWidth, float viewportHeight)
	: viewportWidth(viewportWidth), viewportHeight(viewportHeight)
{
}

Camera::~Camera()
{
}

void Camera::unproject(const Window &window, float viewportX, float viewportY, float viewportWidth, float viewportHeight, XMFLOAT4 *screenCoords) const
{
	float x = screenCoords->x;
	float y = screenCoords->y;
	x = x - viewportX;
	y = window.getSize().y - y - 1.0f;
	y = y - viewportY;
	screenCoords->x = (2.0f * x) / viewportWidth  - 1;
	screenCoords->y = (2.0f * y) / viewportHeight - 1;
	screenCoords->z = 2.0f * screenCoords->z - 1;
	
	XMVECTOR v = XMLoadFloat4(screenCoords);
	v = XMVector4Transform(v, getInvCombinedMatrix());

	XMStoreFloat4(screenCoords, v);
	screenCoords->w = 1.0f / screenCoords->w;
	screenCoords->x *= screenCoords->w;
	screenCoords->y *= screenCoords->w;
	screenCoords->z *= screenCoords->w;
}

void Camera::unproject(const Window &window, XMFLOAT4 *screenCoords) const
{
	unproject(window, 0.0f, 0.0f, (float)window.getSize().x, (float)window.getSize().y, screenCoords);
}

void Camera::project(float viewportX, float viewportY, float viewportWidth, float viewportHeight, XMFLOAT4 *worldCoords) const
{
	XMVECTOR v = XMLoadFloat4(worldCoords);
	v = XMVector4Transform(v, getCombinedMatrix());
	
	XMStoreFloat4(worldCoords, v);
	worldCoords->w = 1.0f / worldCoords->w;
	worldCoords->x *= worldCoords->w;
	worldCoords->y *= worldCoords->w;
	worldCoords->z *= worldCoords->w;

	worldCoords->x = viewportWidth  * (worldCoords->x + 1.0f) / 2.0f + viewportX;
	worldCoords->y = viewportHeight * (worldCoords->y + 1.0f) / 2.0f + viewportY;
	worldCoords->z = (worldCoords->z + 1.0f) / 2.0f;
}

void Camera::project(const Window &window, XMFLOAT4 *worldCoords) const
{
	project(0.0f, 0.0f, (float)window.getSize().x, (float)window.getSize().y, worldCoords);
}
