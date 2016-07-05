#pragma once

#include "GraphicsDevice.h"
#include "RenderTexture.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "ShadowMapSize.h"

class ShadowMap : NonCopyable
{
public:
	DLL_REN_API ShadowMap(GraphicsContext *context, ShadowMapSize size);
	DLL_REN_API ~ShadowMap();

	DLL_REN_API void setRenderTarget(hRenderTarget renderTarget);
	DLL_REN_API void draw(SpriteBatch &batch);

	void renderFullscreenQuad(SpriteBatch &batch, hRenderTarget renderTarget, hTexture2D texture, hPixelShader pixelShader, float alpha) const;
private:
	GraphicsContext *context;
	int shadowMapSize = ShadowMapSize::Default;

	hPixelShader  ComputeDistancesPS;
	hBuffer       ComputeDistancesCB;

	RenderTexture sceneRenderTexture;
	hRenderTarget sceneRenderTarget;
	TextureSize   size;

	RenderTexture distancesRT;
};

