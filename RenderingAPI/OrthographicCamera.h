#pragma once

#include "Window.h"
#include "Camera.h"
#include "AlignedNew.h"
#include "Rect.h"

ALIGN(16)
class OrthographicCamera : public Camera, public AlignedNew<16>
{
public:
	DLL_REN_API OrthographicCamera(float x, float y, float viewportWidth, float viewportHeight, bool update = true);
	DLL_REN_API OrthographicCamera(float viewportWidth, float viewportHeight, bool update = true);
	DLL_REN_API ~OrthographicCamera();
	
	DLL_REN_API void update();
	
	DLL_REN_API void setPosition(float x, float y);
	DLL_REN_API void setViewport(float viewportWidth, float viewportHeight);
	DLL_REN_API void setAngle(float deg);
	DLL_REN_API void setZoom(float zoom);
	
	float getX()     const { return x;     }
	float getY()     const { return y;     }
	float getZoom()  const { return zoom;  }
	float getAngle() const { return angle; }
	
	const DirectX::XMMATRIX& getCombinedMatrix()    const override { return combined;    }
	const DirectX::XMMATRIX& getInvCombinedMatrix() const override { return invCombined; }
private:
	float x     = 0.0f;
	float y     = 0.0f;
	float angle = 0.0f;
	float zoom  = 1.0f;

	DirectX::XMMATRIX combined, invCombined;
};

