#pragma once

#include "GraphicsDevice.h"
#include "RenderTexture.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "ShadowMapSize.h"

class ShadowMap : NonCopyable
{
public:
	DLL_REN_API ShadowMap(GraphicsContext *context,ShadowMapSize size);
	DLL_REN_API ~ShadowMap();

	DLL_REN_API void setRenderTarget(hRenderTarget renderTarget);
	DLL_REN_API void draw(SpriteBatch &batch);
private:
	GraphicsContext *context;

	hPixelShader  ComputeDistancesPS;
	hBuffer       ComputeDistancesCB;

	hRenderTarget shadowRenderTarget;
	TextureSize   size;

	RenderTexture DistancesRT;
};

