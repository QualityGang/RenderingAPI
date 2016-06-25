#pragma once

#include <vector>

#include "GraphicsContext.h"
#include "VertexTypes.h"
#include "Rect.h"
#include "PrimitiveTopology.h"
#include "Camera.h"
#include "AlignedNew.h"
#include "DLLExport.h"

// TODO: Index Buffer
class PrimitiveBatch
{
public:
	DLL_REN_API PrimitiveBatch(GraphicsContext *context, uint32_t batchSize = 2048);
	DLL_REN_API ~PrimitiveBatch();

	DLL_REN_API void begin(
		hRenderTarget     renderTarget,
		const Camera	  &camera,
		PrimitiveTopology topology,
		hVertexShader     customVertexShader = nullptr,
		hPixelShader      customPixelShader = nullptr
	);

	DLL_REN_API void end();
	
	DLL_REN_API void drawPoint(float x, float y, const Color &color);
	DLL_REN_API void drawLine(float x1, float y1, float x2, float y2, const Color &color);
	DLL_REN_API void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Color &color);
	DLL_REN_API void drawRect(const FloatRect &rect, const Color &color);
private:
	typedef VertexPosColor PrimitiveVertex;

	void prepare() const;

	void drawBatch(uint32_t startIndex, uint32_t count) const;
	void transformVertices(PrimitiveVertex *v, uint32_t count) const;

	GraphicsContext *context;
	uint32_t        batchSize;
	uint32_t        numVertsPerPrimitive;
	bool            beginEndPair;

	hRenderTarget     renderTarget;
	const Camera      *camera;
	PrimitiveTopology topology;
	hVertexShader     customVertexShader;
	hPixelShader      customPixelShader;

	hBuffer         vertexBuffer;
	hVertexShader   vertexShader;
	hPixelShader    pixelShader;
	hVertexFormat   vertexFormat;

	std::vector<PrimitiveVertex> vertexList;
};

