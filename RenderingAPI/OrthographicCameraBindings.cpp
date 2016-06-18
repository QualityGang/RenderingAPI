#include "OrthographicCamera.h"

extern "C" DLL_REN_API OrthographicCamera* OrthographicCamera_Create(float x, float y, float viewportWidth, float viewportHeight)
{
	return new OrthographicCamera(x, y, viewportWidth, viewportHeight, false);
}

extern "C" DLL_REN_API void OrthographicCamera_Release(OrthographicCamera *camera)
{
	delete camera;
}

extern "C" DLL_REN_API void OrthographicCamera_Update(OrthographicCamera *camera)
{
	camera->update();
}

extern "C" DLL_REN_API void OrthographicCamera_SetPosition(OrthographicCamera *camera, float x, float y)
{
	camera->setPosition(x, y);
}

extern "C" DLL_REN_API void OrthographicCamera_SetViewport(OrthographicCamera *camera, float viewportWidth, float viewportHeight)
{
	camera->setViewport(viewportWidth, viewportHeight);
}

extern "C" DLL_REN_API void OrthographicCamera_SetAngle(OrthographicCamera *camera, float deg)
{
	camera->setAngle(deg);
}

extern "C" DLL_REN_API void OrthographicCamera_SetZoom(OrthographicCamera *camera, float zoom)
{
	camera->setZoom(zoom);
}

extern "C" DLL_REN_API float OrthographicCamera_GetX(OrthographicCamera *camera)
{
	return camera->getX();
}

extern "C" DLL_REN_API float OrthographicCamera_GetY(OrthographicCamera *camera)
{
	return camera->getY();
}

extern "C" DLL_REN_API float OrthographicCamera_GetZoom(OrthographicCamera *camera)
{
	return camera->getZoom();
}

extern "C" DLL_REN_API float OrthographicCamera_GetAngle(OrthographicCamera *camera)
{
	return camera->getAngle();
}
