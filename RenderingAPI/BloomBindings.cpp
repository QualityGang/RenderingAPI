#include "Bloom.h"

extern "C" DLL_REN_API Bloom* Bloom_Create(GraphicsContext *context)
{
	return new Bloom(context);
}

extern "C" DLL_REN_API void Bloom_Release(Bloom *bloom)
{
	delete bloom;
}

extern "C" DLL_REN_API void Bloom_Apply(Bloom *bloom, SpriteBatch *batch)
{
	bloom->apply(*batch);
}

extern "C" DLL_REN_API void Bloom_SetRenderTarget(Bloom *bloom, void *renderTarget)
{
	bloom->setRenderTarget(hRenderTarget(renderTarget));
}

extern "C" DLL_REN_API void* Bloom_GetRenderTarget(Bloom *bloom)
{
	return bloom->getRenderTarget().as<void>();
}

extern "C" DLL_REN_API void Bloom_SetSettings(Bloom *bloom, BloomSettings settings)
{
	bloom->setSettings(settings);
}

extern "C" DLL_REN_API BloomSettings Bloom_GetSettings(Bloom *bloom)
{
	return bloom->getSettings();
}