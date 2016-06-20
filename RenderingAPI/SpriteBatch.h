#pragma once

#include <vector>

#include "GraphicsContext.h"
#include "FontAtlas.h"
#include "Sprite.h"
#include "Text.h"
#include "Camera.h"
#include "SpriteSoftMode.h"
#include "AlignedNew.h"
#include "NonCopyable.h"
#include "DLLExport.h"

class SpriteBatch : public NonCopyable
{
public:
	DLL_REN_API SpriteBatch(GraphicsContext *context, uint32_t batchSize = 2048);
	DLL_REN_API ~SpriteBatch();

	DLL_REN_API void begin(
		hRenderTarget      renderTarget,
		const Camera	   &camera,
		SpriteSortMode	   sortMode           = SpriteSortMode_Deferred,
		hBlendState		   blendState         = nullptr,
		hSamplerState      samplerState       = nullptr,
		hDepthStencilState depthStencilState  = nullptr,
		hRasterizerState   rasterizerState    = nullptr,
		hVertexShader      customVertexShader = nullptr,
		hPixelShader       customPixelShader  = nullptr
	);

	DLL_REN_API void end();

	DLL_REN_API void draw(const Sprite &sprite);
	DLL_REN_API void draw(const Text &text, const FontAtlas &atlas);
private:
	ALIGN(16)
	struct SpriteVertex : public AlignedNew<16>
	{
		SpriteVertex(float x, float y, float z,
			float u, float v,
			float r, float g, float b, float a)
			: position(x, y, z, 1.0f), texCoords(u, v), color(r, g, b, a)
		{
		}

		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texCoords;
		DirectX::XMFLOAT4 color;
	};

	void prepare()  const;
	void finalize() const;

	void     drawBatch(uint32_t startIndex, uint32_t count) const;
	uint32_t getBatchSize(uint32_t startIndex, uint32_t endIndex) const;
	void     transformVertices(const Sprite &sprite, SpriteVertex *v, uint32_t count) const;
	
	GraphicsContext *context;
	uint32_t      batchSize;
	const Camera *camera;
	bool          beginEndPair;

	hRenderTarget      renderTarget;
	SpriteSortMode     sortMode;
	hBlendState		   blendState;
	hSamplerState      samplerState;
	hDepthStencilState depthStencilState;
	hRasterizerState   rasterizerState;
	hVertexShader      customVertexShader;
	hPixelShader       customPixelShader;

	hBuffer         vertexBuffer, indexBuffer;
	hVertexShader   vertexShader;
	hPixelShader    pixelShader;
	hVertexFormat   vertexFormat;

	std::vector<Sprite> spriteList;

	static bool Immediate;

	hBlendState		   defBlendState;
	hSamplerState      defSamplerState;
	hDepthStencilState defDepthStencilState;
	hRasterizerState   defRasterizerState;
};

