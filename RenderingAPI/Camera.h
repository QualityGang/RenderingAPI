#pragma once

#include <DirectXMath.h>

#include "DLLExport.h"

class Window;

class Camera
{
public:
	DLL_REN_API Camera(float viewportWidth, float viewportHeight);
	DLL_REN_API ~Camera();

	DLL_REN_API void unproject(const Window &window, float viewportX, float viewportY, float viewportWidth, float viewportHeight, DirectX::XMFLOAT4 *screenCoords) const;
	DLL_REN_API void unproject(const Window &window, DirectX::XMFLOAT4 *screenCoords) const;
	DLL_REN_API void project(float viewportX, float viewportY, float viewportWidth, float viewportHeight, DirectX::XMFLOAT4 *worldCoords) const;
	DLL_REN_API void project(const Window &window, DirectX::XMFLOAT4 *worldCoords) const;

	float getViewportWidth()  const { return viewportWidth;  }
	float getViewportHeight() const { return viewportHeight; }

	DLL_REN_API virtual const DirectX::XMMATRIX& getView() const = 0;
	DLL_REN_API virtual const DirectX::XMMATRIX& getProjection() const = 0;
	DLL_REN_API virtual const DirectX::XMMATRIX& getCombinedMatrix() const = 0;
	DLL_REN_API virtual const DirectX::XMMATRIX& getInvCombinedMatrix() const = 0;
protected:
	float viewportWidth, viewportHeight;
};

