#pragma once

#include "GraphicsDevice.h"
#include "RenderTexture.h"
#include "Sprite.h"
#include "Camera.h"
#include "OrthographicCamera.h"

class ShadowMap
{
public:
	DLL_REN_API ShadowMap(GraphicsContext *context, uint32_t width, uint32_t height);
	DLL_REN_API ~ShadowMap();

	DLL_REN_API void update(const Window &window, OrthographicCamera &camera);

	Sprite& getShadowSprite() { return shadowSprite; }
	hTexture2D getTexture2D() const { return renderTexture.getTexture2D(); }
private:
	GraphicsContext *context;

	Sprite        shadowSprite;
	RenderTexture renderTexture;
};

