#include "SpriteBatch.h"

extern "C" DLL_REN_API SpriteBatch* SpriteBatch_Create(GraphicsContext *context)
{
	return new SpriteBatch(context);
}

extern "C" DLL_REN_API void SpriteBatch_Release(SpriteBatch *batch)
{
	delete batch;
}

extern "C" DLL_REN_API void SpriteBatch_Begin(SpriteBatch *batch, void *renderTarget, Camera *camera)
{
	batch->begin(hRenderTarget(renderTarget), *camera);
}

extern "C" DLL_REN_API void SpriteBatch_End(SpriteBatch *batch)
{
	batch->end();
}

extern "C" DLL_REN_API void SpriteBatch_Draw(SpriteBatch *batch, Sprite *sprite)
{
	batch->draw(*sprite);
}
