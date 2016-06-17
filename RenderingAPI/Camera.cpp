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

void Camera::project(XMFLOAT4 &vec) const
{
	XMVECTOR vector = XMVectorSet(vec.x, vec.y, vec.z, vec.w);
	vector = XMVector4Transform(vector, getCombinedMatrix());

	XMStoreFloat4(&vec, vector);
	vec.x = ((vec.x / vec.w) * 0.5f + 0.5f) * viewportWidth + 0;
	vec.y = (1.0f - ((vec.y / vec.w) * 0.5f + 0.5f)) * viewportHeight + 0;
}

void Camera::unproject(const Window &window, XMFLOAT4 &vec) const
{
	XMVECTOR vector = XMVectorSet(
		(2.0f * ((float)(vec.x - 0) / (window.getSize().x - 0))) - 1.0f,
		1.0f - (2.0f * ((float)(vec.y - 0) / (window.getSize().y - 0))),
		2.0f * vec.z - 1.0f,
		vec.w
	);

	vector = XMVector4Transform(vector, getInvCombinedMatrix());

	XMStoreFloat4(&vec, vector);
	vec.w = 1.0f / vec.w;
	vec.x *= vec.w;
	vec.y *= vec.w;
	vec.z *= vec.w;
}