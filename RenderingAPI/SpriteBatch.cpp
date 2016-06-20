#include "SpriteBatch.h"

#include <algorithm>

#include "Throw.h"

#include "stringex.h"

#include "SpriteVS.h"
#include "SpritePS.h"

using namespace DirectX;

#define NUM_VERTICES 4
#define NUM_INDICES  6

bool SpriteBatch::Immediate = false;

SpriteBatch::SpriteBatch(GraphicsContext *context, uint32_t batchSize)
	: context(context), batchSize(batchSize)
{
	spriteList.reserve(batchSize);

	// create shaders
	vertexShader = context->createVertexShader(g_SpriteVS, sizeof(g_SpriteVS));
	pixelShader  = context->createPixelShader(g_SpritePS, sizeof(g_SpritePS));

	// create vertex format
	uint32_t vfStride0 = 0;
	uint32_t vfStride1 = vfStride0 + context->getPixelFormatStride(PixelFormat_RGBA32F);
	uint32_t vfStride2 = vfStride1 + context->getPixelFormatStride(PixelFormat_RG32F);

	VertexElement elements[] =
	{
		{ "POSITION", 0, PixelFormat_RGBA32F, 0, vfStride0, InputDataType_PerVertex, 0 },
		{ "TEXCOORD", 0, PixelFormat_RG32F,   0, vfStride1, InputDataType_PerVertex, 0 },
		{ "COLOR",    0, PixelFormat_RGBA32F, 0, vfStride2, InputDataType_PerVertex, 0 }
	};

	vertexFormat = context->createVertexFormat(elements, 3, g_SpriteVS, sizeof(g_SpriteVS));

	// create vertex buffer
	vertexBuffer = context->createBuffer(BufferType_VertexBuffer, sizeof(SpriteVertex) * batchSize * NUM_VERTICES, AccessFlag_Write, nullptr);

	// create index buffer
	if (batchSize * (uint64_t)NUM_VERTICES > UINT_MAX)
		THROW("batch size too large");

	std::vector<uint32_t> indices;
	indices.reserve(batchSize * NUM_INDICES);

	for (uint32_t i = 0; i < batchSize * NUM_VERTICES; i += NUM_VERTICES)
	{
		indices.push_back(i + 0);
		indices.push_back(i + 1);
		indices.push_back(i + 2);

		indices.push_back(i + 0);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
	}
	
	indexBuffer = context->createBuffer(BufferType_IndexBuffer,
		sizeof(uint32_t) * batchSize * NUM_INDICES, AccessFlag_None, &indices[0]);

	// create default states
	BlendState blendState; // Alpha Blend
	blendState.blendDesc[0].blendEnabled  = true;
	blendState.blendDesc[0].srcBlend	  = BlendFactor_SrcAlpha;
	blendState.blendDesc[0].srcBlendAlpha = BlendFactor_One;
	blendState.blendDesc[0].dstBlend	  = BlendFactor_OneMinusSrcAlpha;
	blendState.blendDesc[0].dstBlendAlpha = BlendFactor_Zero;
	blendState.blendDesc[0].blendOp		  = BlendOp_Add;
	blendState.blendDesc[0].blendOpAlpha  = BlendOp_Add;
	blendState.blendDesc[0].writeMask     = ColorWriteMask_All;

	defBlendState        = context->createBlendState(blendState);
	defSamplerState      = context->createSamplerState(SamplerState());
	defDepthStencilState = context->createDepthStencilState(DepthStencilState());
	defRasterizerState   = context->createRasterizerState(RasterizerState());
}

SpriteBatch::~SpriteBatch()
{
	context->releaseBuffer(vertexBuffer);
	context->releaseBuffer(indexBuffer);
	context->releaseVertexShader(vertexShader);
	context->releasePixelShader(pixelShader);
	context->releaseVertexFormat(vertexFormat);

	context->releaseBlendState(defBlendState);
	context->releaseSamplerState(defSamplerState);
	context->releaseDepthStencilState(defDepthStencilState);
	context->releaseRasterizerState(defRasterizerState);
}

void SpriteBatch::begin(
	hRenderTarget      renderTarget,
	const Camera	   &camera,
	SpriteSortMode	   sortMode,
	hBlendState		   blendState,
	hSamplerState      samplerState,
	hDepthStencilState depthStencilState,
	hRasterizerState   rasterizerState,
	hVertexShader      customVertexShader,
	hPixelShader       customPixelShader 
)
{
	if (beginEndPair)
		return;

	if (Immediate)
		return;

	beginEndPair = true;

	this->renderTarget       = renderTarget;
	this->camera             = &camera;
	this->sortMode           = sortMode;
	this->blendState         = blendState;
	this->samplerState       = samplerState;
	this->depthStencilState  = depthStencilState;
	this->rasterizerState    = rasterizerState;
	this->customVertexShader = customVertexShader;
	this->customPixelShader  = customPixelShader;

	spriteList.clear();

	if (sortMode == SpriteSortMode_Immediate)
	{
		Immediate = true;
		prepare();
	}
}

void SpriteBatch::end()
{
	if (!beginEndPair)
		return;

	beginEndPair = false;

	// sort the sprites
	switch (sortMode)
	{
		case SpriteSortMode_Texture:
			std::sort(spriteList.begin(), spriteList.end(),
			[](Sprite &sprite1, Sprite &sprite2)
			{
				return sprite1.getTexture().as<void>() < sprite2.getTexture().as<void>();
			});
			break;

		case SpriteSortMode_BackToFront:
			std::sort(spriteList.begin(), spriteList.end(),
			[](Sprite &sprite1, Sprite &sprite2)
			{
				return sprite1.getDepth() > sprite2.getDepth();
			});
			break;
		case SpriteSortMode_FrontToBack:
			std::sort(spriteList.begin(), spriteList.end(),
			[](Sprite &sprite1, Sprite &sprite2)
			{
				return sprite1.getDepth() < sprite2.getDepth();
			});
			break;

		case SpriteSortMode_Immediate:
			Immediate = false;
			finalize();
			return;
	}

	prepare();

	// draw batches
	size_t   numSpritesToDraw = spriteList.size();
	uint32_t startIndex       = 0;

	while (numSpritesToDraw > 0)
	{
		uint32_t count = getBatchSize(startIndex, std::min(startIndex + batchSize, (uint32_t)spriteList.size()));
		context->setPSTexture2Ds(&spriteList[startIndex].getTexture(), 0, 1);

		while (count > 0)
		{
			if (count > batchSize)
			{
				drawBatch(startIndex, batchSize);
				numSpritesToDraw -= batchSize;
				startIndex += batchSize;
				count -= batchSize;
			}
			else
			{
				drawBatch(startIndex, count);
				numSpritesToDraw -= count;
				startIndex += count;
				count = 0;
			}
		}
	}

	finalize();
}

void SpriteBatch::prepare() const
{
	// configure pipeline
	uint32_t stride = sizeof(SpriteVertex);
	uint32_t offset = 0;

	TextureSize vpSize;
	hTexture2D surface = context->getTexture2D(renderTarget, 0);
	context->getTexture2DSize(surface, &vpSize);

	context->setVertexBuffers(&vertexBuffer, 0, 1, &stride, &offset);
	context->setIndexBuffer(indexBuffer, 0);

	context->setVertexShader(customVertexShader ? customVertexShader : vertexShader);
	context->setPixelShader(customPixelShader ? customPixelShader : pixelShader);
	context->setVertexFormat(vertexFormat);
	
	context->setBlendState(blendState ? blendState : defBlendState);
	context->setPSSamplers((samplerState ? &samplerState : &defSamplerState), 0, 1);
	context->setDepthStencilState(depthStencilState ? depthStencilState : defDepthStencilState);
	context->setRasterizerState(rasterizerState ? rasterizerState : defRasterizerState);
	
	context->setRenderTarget(renderTarget);
	context->setViewport((float)vpSize.width, (float)vpSize.height, 0.0f, 1.0f);
	context->setPrimitiveTopology(PrimitiveTopology_TriangleList);

	context->releaseTexture2D(surface);
}

void SpriteBatch::finalize() const
{
	hTexture2D nullTexture;
	context->setPSTexture2Ds(&nullTexture, 0, 1);
}

void SpriteBatch::drawBatch(uint32_t startIndex, uint32_t count) const
{
	// update vertex buffer
	MapData mapData;
	context->mapBuffer(vertexBuffer, MapType_Write, &mapData);
	SpriteVertex *vertBuffMap = (SpriteVertex*)mapData.mem;

	for (uint32_t i = 0; i < count; i++)
	{
		const Sprite &sprite = spriteList[startIndex + i];

		TextureSize texSize;
		context->getTexture2DSize(sprite.getTexture(), &texSize);

		if (texSize.width == 0 || texSize.height == 0)
			continue;

		float srcLeftNorm   = sprite.getSrcRect().x / texSize.width;
		float srcTopNorm    = sprite.getSrcRect().y / texSize.height;
		float srcRightNorm  = srcLeftNorm + sprite.getSrcRect().width  / texSize.width;
		float srcBottomNorm = srcTopNorm  + sprite.getSrcRect().height / texSize.height;

		if (sprite.getEffect() & SpriteEffect_FlipVertical)
			std::swap(srcTopNorm, srcBottomNorm);

		if (sprite.getEffect() & SpriteEffect_FlipHorizontal)
			std::swap(srcLeftNorm, srcRightNorm);

		vertBuffMap[i * NUM_VERTICES + 0] = SpriteVertex(0.0f, 1.0f, sprite.getDepth(), srcLeftNorm,  srcBottomNorm, sprite.getColor().r / 255.0f, sprite.getColor().g / 255.0f, sprite.getColor().b / 255.0f, sprite.getColor().a / 255.0f); // bot left
		vertBuffMap[i * NUM_VERTICES + 1] = SpriteVertex(0.0f, 0.0f, sprite.getDepth(), srcLeftNorm,  srcTopNorm,    sprite.getColor().r / 255.0f, sprite.getColor().g / 255.0f, sprite.getColor().b / 255.0f, sprite.getColor().a / 255.0f); // top left
		vertBuffMap[i * NUM_VERTICES + 2] = SpriteVertex(1.0f, 0.0f, sprite.getDepth(), srcRightNorm, srcTopNorm,    sprite.getColor().r / 255.0f, sprite.getColor().g / 255.0f, sprite.getColor().b / 255.0f, sprite.getColor().a / 255.0f); // top right
		vertBuffMap[i * NUM_VERTICES + 3] = SpriteVertex(1.0f, 1.0f, sprite.getDepth(), srcRightNorm, srcBottomNorm, sprite.getColor().r / 255.0f, sprite.getColor().g / 255.0f, sprite.getColor().b / 255.0f, sprite.getColor().a / 255.0f); // bot right

		transformVertices(sprite, &vertBuffMap[i * NUM_VERTICES], NUM_VERTICES);
	}

	context->unmapBuffer(vertexBuffer);
	context->drawIndexed(count * NUM_INDICES, 0, 0);
}

uint32_t SpriteBatch::getBatchSize(uint32_t startIndex, uint32_t endIndex) const
{
	uint32_t count = 1;
	hTexture2D texture = spriteList[startIndex].getTexture();

	for (uint32_t i = startIndex + 1; i < endIndex; i++)
	{
		if (texture != spriteList[i].getTexture())
			break;

		count++;
	}

	return count;
}

void SpriteBatch::transformVertices(const Sprite &sprite, SpriteVertex *v, uint32_t count) const
{
	XMVECTOR scaling = XMVectorSet(sprite.getWidth(), sprite.getHeight(), 1.0f, 1.0f);
	XMVECTOR origin = XMVectorSet(sprite.getWidth() / 2.0f, sprite.getHeight() / 2.0f, 0.0f, 0.0f);
	XMVECTOR translation = XMVectorSet(sprite.getX(), sprite.getY(), sprite.getDepth(), 0.0f);

	XMMATRIX worldMatrix = XMMatrixAffineTransformation2D(scaling, origin, -sprite.getAngle(), translation);

	XMMATRIX transform = worldMatrix * camera->getCombinedMatrix();

	for (uint32_t i = 0; i < count; i++)
	{
		XMVECTOR p = XMLoadFloat4(&v[i].position);
		p = XMVector4Transform(p, transform);
		XMStoreFloat4(&v[i].position, p);
	}
}

void SpriteBatch::draw(const Sprite &sprite)
{
	if (!beginEndPair)
		return;

	if (sortMode == SpriteSortMode_Immediate)
	{
		if (spriteList.empty())
			spriteList.push_back(sprite);
		else
			spriteList[0] = sprite;
	
		context->setPSTexture2Ds(&sprite.getTexture(), 0, 1);
		drawBatch(0, 1);

		return;
	}

	spriteList.push_back(sprite);
}

void SpriteBatch::draw(const Text &text, const FontAtlas &atlas)
{
	if (!beginEndPair)
		return;

	const Font *font = atlas.getFont();

	if (!font)
		return;

	FT_Face face = font->getFace();

	FT_UInt prevGlyphIndex = 0;
	XMFLOAT2 pen(0, 0);

	std::vector<std::string> lines;
	stdex::split_string(text.getString(), "\n", &lines);

	uint32_t currLine = 0;
	int maxAscent  = atlas.getMaxAscent(lines[currLine].c_str());
	int maxDescent = atlas.getMaxDescent(lines[currLine].c_str());

	for (const char *lpStr = text.getString(); *lpStr; lpStr++)
	{
		switch (*lpStr)
		{
			case ' ':
				pen.x += atlas.getCharInfo(' ')->advance.x;
				continue;
			case '\t':
				pen.x += atlas.getCharInfo(' ')->advance.x * 3;
				continue;
			case '\n':
				pen.x = 0;
				pen.y += text.getLineGap() + maxDescent + maxAscent;
				currLine++;
				maxAscent  = atlas.getMaxAscent(lines[currLine].c_str());
				maxDescent = atlas.getMaxDescent(lines[currLine].c_str());
				continue;
		}

		const FontAtlas::CharInfo *charInfo = atlas.getCharInfo(*lpStr);

		if (!charInfo)
		{
			pen.x += atlas.getCharInfo(' ')->advance.x;
			continue;
		}

		FT_UInt glyphIndex = FT_Get_Char_Index(face, *lpStr);
		pen.x += font->getKerning(prevGlyphIndex, glyphIndex);

		Sprite sprite;
		sprite.setPosition(text.getX() + pen.x + charInfo->rect.x, 
						   text.getY() + pen.y - charInfo->rect.y + maxAscent);
		sprite.setSize((float)charInfo->rect.width, (float)charInfo->rect.height);
		sprite.setSrcRect(FloatRect((float)charInfo->charPos.x,  (float)charInfo->charPos.y,
									(float)charInfo->rect.width, (float)charInfo->rect.height));
		sprite.setDepth(text.getDepth());
		sprite.setColor(text.getColor());
		sprite.setTexture(atlas.getBitmap().getTexture2D());

		draw(sprite);

		pen.x += charInfo->advance.x;
		prevGlyphIndex = glyphIndex;
	}
}