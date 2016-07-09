#pragma once

#include "GraphicsDevice.h"
#include "RenderTexture.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "ShadowMapSize.h"
#include <vector>

using std::vector;

class ShadowMap : NonCopyable
{
public:
	DLL_REN_API ShadowMap(GraphicsContext *context, ShadowMapSize size);
	DLL_REN_API ~ShadowMap();

	DLL_REN_API void setRenderTarget(hRenderTarget renderTarget);
	DLL_REN_API void ApplyReduction(SpriteBatch &batch, RenderTexture *source, RenderTexture* destination);
	DLL_REN_API void draw(SpriteBatch &batch, Color backgroundColor);

	void renderFullscreenQuad(SpriteBatch &batch, hRenderTarget destination, hTexture2D source, hPixelShader pixelShader, float alpha) const;
private:
	GraphicsContext *context;
	int shadowMapSize = 2 << ShadowMapSize::Default;
	int reductionChainCount = ShadowMapSize::Default;

	RenderTexture sceneRenderTexture;
	hRenderTarget sceneRenderTarget;
	TextureSize   size;

	hPixelShader  computeDistancesPS;
	hBuffer       computeDistancesCB;

	hPixelShader  distortPS;
	hBuffer       distortCB;

	hPixelShader  hReductionPS;
	hBuffer       hReductionCB;

	hPixelShader  shadowPS;
	hBuffer       shadowCB;

	RenderTexture distancesRT;
	RenderTexture distortRT;
	vector<RenderTexture*> reductionRT;
	RenderTexture shadowMapRT;
};

