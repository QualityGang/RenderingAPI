#pragma once

#include <vector>

#include "GraphicsContext.h"
#include "FontAtlas.h"
#include "Sprite.h"
#include "Text.h"
#include "Camera.h"
#include "SpriteSortMode.h"
#include "VertexTypes.h"
#include "NonCopyable.h"
#include "DLLExport.h"

class SpriteBatch : public NonCopyable
{
public:
	DLL_REN_API SpriteBatch(GraphicsContext *context, uint32_t batchSize = 2048);
	DLL_REN_API ~SpriteBatch();

	DLL_REN_API void begin(
		hRenderTarget      renderTarget,
		SpriteSortMode	   sortMode           = SpriteSortMode_Deferred,
		const Camera	   *camera            = nullptr,
		hVertexShader      customVertexShader = nullptr,
		hPixelShader       customPixelShader  = nullptr
	);

	DLL_REN_API void end();

	DLL_REN_API void draw(const Sprite &sprite);
	DLL_REN_API void draw(const Text &text, const FontAtlas &atlas);
private:
	typedef VertexPosTexColor SpriteVertex;

	void prepare() const;
	void finalize() const;

	void     drawBatch(uint32_t startIndex, uint32_t count) const;
	uint32_t getBatchSize(uint32_t startIndex, uint32_t endIndex) const;
	void     transformVertices(const Sprite &sprite, SpriteVertex *v, uint32_t count) const;
	
	GraphicsContext *context;
	uint32_t        batchSize;
	bool            beginEndPair;

	hRenderTarget      renderTarget;
	SpriteSortMode     sortMode;
	DirectX::XMMATRIX  projection;
	hVertexShader      customVertexShader;
	hPixelShader       customPixelShader;

	TextureSize sceneSize;

	hBuffer         vertexBuffer, indexBuffer;
	hVertexShader   vertexShader;
	hPixelShader    pixelShader;
	hVertexFormat   vertexFormat;

	std::vector<Sprite> spriteList;

	static bool Immediate;
};

