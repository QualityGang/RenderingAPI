#pragma once

#include "GraphicsContext.h"
#include "SpriteBatch.h"
#include "RenderTexture.h"
#include "BloomSettings.h"
#include "NonCopyable.h"
#include "DLLExport.h"

class Bloom : public NonCopyable
{
public:
	DLL_REN_API Bloom(GraphicsContext *context);
	DLL_REN_API ~Bloom();

	enum ShowFilter : int32_t
	{
		InitialImage = -1,
		PreBloom,
		BlurredHorizontally,
		BlurredBothWays,
		FinalResult
	};

	DLL_REN_API void apply(SpriteBatch &batch) const;

	DLL_REN_API void setRenderTarget(hRenderTarget renderTarget);
	hRenderTarget getRenderTarget() const { return sceneRenderTarget; }

	void setSettings(const BloomSettings &settings) { this->settings = settings; }
	const BloomSettings& getSettings() const { return settings; }

	void setShowFilter(ShowFilter showFilter) { this->showFilter = showFilter; };
	ShowFilter getShowFilter() const { return showFilter; }
private:
	void renderFullscreenQuad(SpriteBatch &batch, hRenderTarget renderTarget, hTexture2D texture, hPixelShader pixelShader, ShowFilter currentShowFilter) const;
	void setBlurParameters(float dx, float dy) const;
	float computeGaussian(float n) const;

	GraphicsContext *context;

	hPixelShader bloomExtractPS;
	hBuffer      bloomExtractCB;

	hPixelShader gaussianBlurPS;
	hBuffer      gaussianBlurCB;

	hPixelShader bloomCombinePS;
	hBuffer		 bloomCombineCB;

	hRenderTarget sceneRenderTarget;
	TextureSize   sceneSize;

	RenderTexture sceneRenderTexture;
	RenderTexture renderTexture1;
	RenderTexture renderTexture2;

	BloomSettings settings = BloomSettings::Default;

	ShowFilter showFilter = FinalResult;
};

