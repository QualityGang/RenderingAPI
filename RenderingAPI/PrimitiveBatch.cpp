#include "PrimitiveBatch.h"

#include "PrimitiveVS.h"
#include "PrimitivePS.h"

#include "Throw.h"

using namespace DirectX;

PrimitiveBatch::PrimitiveBatch(GraphicsContext *context, uint32_t batchSize)
	: context(context), batchSize(batchSize)
{
	vertexList.reserve(batchSize);

	// create shaders
	vertexShader = context->createVertexShader(g_PrimitiveVS, sizeof(g_PrimitiveVS));
	pixelShader = context->createPixelShader(g_PrimitivePS, sizeof(g_PrimitivePS));

	// create vertex format
	uint32_t vfStride0 = 0;
	uint32_t vfStride1 = vfStride0 + context->getPixelFormatStride(PixelFormat_RGBA32F);

	VertexElement elements[] =
	{
		{ "POSITION", 0, PixelFormat_RGBA32F, 0, vfStride0, InputDataType_PerVertex, 0 },
		{ "COLOR",    0, PixelFormat_RGBA32F, 0, vfStride1, InputDataType_PerVertex, 0 }
	};

	vertexFormat = context->createVertexFormat(elements, 2, g_PrimitiveVS, sizeof(g_PrimitiveVS));

	// create vertex buffer
	vertexBuffer = context->createBuffer(BufferType_VertexBuffer, sizeof(PrimitiveVertex) * batchSize, AccessFlag_Write, nullptr);
}

PrimitiveBatch::~PrimitiveBatch()
{
	context->releaseBuffer(vertexBuffer);
	context->releaseVertexShader(vertexShader);
	context->releasePixelShader(pixelShader);
	context->releaseVertexFormat(vertexFormat);
}

void PrimitiveBatch::begin(
	hRenderTarget     renderTarget,
	PrimitiveTopology topology,
	const Camera	  *camera,
	hVertexShader     customVertexShader,
	hPixelShader      customPixelShader)
{
	if (beginEndPair)
		return;

	beginEndPair = true;

	switch (topology)
	{
		case PrimitiveTopology_PointList:
			numVertsPerPrimitive = 1;
			break;

		case PrimitiveTopology_LineList:
			numVertsPerPrimitive = 2;
			break;

		case PrimitiveTopology_TriangleList:
			numVertsPerPrimitive = 3;
			break;

		default:
			THROW("PrimitiveTopology is not valid");
	}

	context->getRenderTargetSize(renderTarget, 0, &sceneSize);

	this->renderTarget = renderTarget;
	this->topology = topology;
	this->projection = camera ? camera->getCombinedMatrix() : XMMatrixOrthographicOffCenterLH(0.0f, (float)sceneSize.width, (float)sceneSize.height, 0.0f, 0.0f, 1.0f);
	this->customVertexShader = customVertexShader;
	this->customPixelShader = customPixelShader;

	vertexList.clear();
}

void PrimitiveBatch::end()
{
	if (!beginEndPair)
		return;

	beginEndPair = false;

	prepare();

	// draw batches
	uint32_t numVerticesToDraw = (uint32_t)vertexList.size();
	uint32_t validBatchSize = batchSize - (batchSize % numVertsPerPrimitive);
	uint32_t startIndex = 0;
	
	assert(validBatchSize % numVertsPerPrimitive == 0);

	while (numVerticesToDraw > 0)
	{
		// if a primitive doesnt have enough vertices then skip it
		uint32_t extraVertices = numVerticesToDraw % numVertsPerPrimitive;
		numVerticesToDraw -= extraVertices;
		startIndex += extraVertices;
		
		if (numVerticesToDraw > validBatchSize)
		{
			drawBatch(startIndex, validBatchSize);
			numVerticesToDraw -= validBatchSize;
			startIndex += validBatchSize;
		}
		else
		{
			drawBatch(startIndex, numVerticesToDraw);
			numVerticesToDraw = 0;
			startIndex += numVerticesToDraw;
		}
	}
}

void PrimitiveBatch::drawPoint(float x, float y, const Color &color)
{
	if (!beginEndPair)
		return;

	vertexList.push_back(VertexPosColor(x, y, 0.0f, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f));
}

void PrimitiveBatch::drawLine(float x1, float y1, float x2, float y2, const Color &color)
{
	drawPoint(x1, y1, color);
	drawPoint(x2, y2, color);
}

void PrimitiveBatch::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Color &color)
{
	drawPoint(x1, y1, color);
	drawPoint(x2, y2, color);
	drawPoint(x3, y3, color);
}

void PrimitiveBatch::drawRect(const FloatRect &rect, const Color &color)
{
	drawPoint(rect.x, rect.y, color);
	drawPoint(rect.x + rect.width, rect.y, color);
	drawPoint(rect.x + rect.width, rect.y + rect.height, color);
	drawPoint(rect.x, rect.y, color);
	drawPoint(rect.x + rect.width, rect.y + rect.height, color);
	drawPoint(rect.x, rect.y + rect.height, color);
}

void PrimitiveBatch::prepare() const
{
	// configure pipeline
	uint32_t stride = sizeof(PrimitiveVertex);
	uint32_t offset = 0;

	TextureSize vpSize;
	context->getRenderTargetSize(renderTarget, 0, &vpSize);

	context->setVertexBuffers(&vertexBuffer, 0, 1, &stride, &offset);

	context->setVertexShader(customVertexShader ? customVertexShader : vertexShader);
	context->setPixelShader(customPixelShader ? customPixelShader : pixelShader);
	context->setVertexFormat(vertexFormat);

	context->setRenderTarget(renderTarget);
	context->setViewport((float)vpSize.width, (float)vpSize.height, 0.0f, 1.0f);
	context->setPrimitiveTopology(topology);
}

void PrimitiveBatch::drawBatch(uint32_t startIndex, uint32_t count) const
{
	// update vertex buffer
	MapData mapData;
	context->mapBuffer(vertexBuffer, MapType_Write, &mapData);
	PrimitiveVertex *vertBuffMap = (PrimitiveVertex*)mapData.mem;

	for (uint32_t i = 0; i < count; i++)
		vertBuffMap[i] = vertexList[startIndex + i];

	transformVertices(vertBuffMap, count);

	context->unmapBuffer(vertexBuffer);
	context->draw(count, 0);
}

void PrimitiveBatch::transformVertices(PrimitiveVertex *v, uint32_t count) const
{
	XMMATRIX transform = projection;

	for (uint32_t i = 0; i < count; i++)
	{
		XMVECTOR p = XMLoadFloat4(&v[i].position);
		p = XMVector4Transform(p, transform);
		XMStoreFloat4(&v[i].position, p);
	}
}
