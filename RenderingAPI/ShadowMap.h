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

	DLL_REN_API void Update(OrthographicCamera *camera);

	DLL_REN_API Sprite getShadowSprite() const;
	DLL_REN_API RenderTexture getRenderTexture() const;
private:
	GraphicsContext *context;

	Sprite shadowSprite;
	RenderTexture renderTexture;
};

