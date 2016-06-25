#include "GraphicsContext.h"

GraphicsContext::GraphicsContext()
{
}

GraphicsContext::~GraphicsContext()
{
}

void GraphicsContext::init(uint32_t maxAnisotropy)
{
	// blend states
	BlendState bsAlphaBlend;
	bsAlphaBlend.blendDesc[0].blendEnabled  = true;
	bsAlphaBlend.blendDesc[0].srcBlend      = BlendFactor_SrcAlpha;
	bsAlphaBlend.blendDesc[0].srcBlendAlpha = BlendFactor_One;
	bsAlphaBlend.blendDesc[0].dstBlend      = BlendFactor_OneMinusSrcAlpha;
	bsAlphaBlend.blendDesc[0].dstBlendAlpha = BlendFactor_Zero;
	bsAlphaBlend.blendDesc[0].blendOp       = BlendOp_Add;
	bsAlphaBlend.blendDesc[0].blendOpAlpha  = BlendOp_Add;
	bsAlphaBlend.blendDesc[0].writeMask     = ColorWriteMask_All;

	BSAlphaBlend = createBlendState(bsAlphaBlend);

	BlendState bsAdditive;
	bsAdditive.blendDesc[0].blendEnabled  = true;
	bsAdditive.blendDesc[0].srcBlend      = BlendFactor_One;
	bsAdditive.blendDesc[0].srcBlendAlpha = BlendFactor_One;
	bsAdditive.blendDesc[0].dstBlend      = BlendFactor_One;
	bsAdditive.blendDesc[0].dstBlendAlpha = BlendFactor_One;
	bsAdditive.blendDesc[0].blendOp       = BlendOp_Add;
	bsAdditive.blendDesc[0].blendOpAlpha  = BlendOp_Add;
	bsAdditive.blendDesc[0].writeMask     = ColorWriteMask_All;

	BSAdditive = createBlendState(bsAdditive);

	//BlendState nonPremultiplied;

	BlendState bsOpaque;
	bsOpaque.blendDesc[0].blendEnabled  = true;
	bsOpaque.blendDesc[0].srcBlend      = BlendFactor_One;
	bsOpaque.blendDesc[0].srcBlendAlpha = BlendFactor_One;
	bsOpaque.blendDesc[0].dstBlend      = BlendFactor_Zero;
	bsOpaque.blendDesc[0].dstBlendAlpha = BlendFactor_Zero;
	bsOpaque.blendDesc[0].blendOp       = BlendOp_Add;
	bsOpaque.blendDesc[0].blendOpAlpha  = BlendOp_Add;
	bsOpaque.blendDesc[0].writeMask     = ColorWriteMask_All;

	BSOpaque = createBlendState(bsOpaque);

	// depth stencil states
	DSSDefault = createDepthStencilState(DepthStencilState());

	DepthStencilState dssRead;
	dssRead.depthEnabled = true;
	dssRead.writeMask    = DepthWriteMask_Zero;

	DSSRead = createDepthStencilState(dssRead);

	DepthStencilState dssNone;
	dssNone.depthEnabled = false;
	dssNone.writeMask    = DepthWriteMask_Zero;

	DSSNone = createDepthStencilState(dssNone);

	// rasterizer states
	RasterizerState rsCullClockwise;
	rsCullClockwise.cullMode = CullMode_Front;

	RSCullClockwise = createRasterizerState(rsCullClockwise);

	RSCullCounterClockwise = createRasterizerState(RasterizerState());

	RasterizerState rsCullNone;
	rsCullNone.cullMode = CullMode_None;

	RSCullNone = createRasterizerState(rsCullNone);

	RasterizerState rsWireframe;
	rsWireframe.cullMode = CullMode_None;
	rsWireframe.fillMode = FillMode_Wireframe;

	RSWireframe = createRasterizerState(rsWireframe);

	// sampler states
	SamplerState ssPointWrap;
	ssPointWrap.filter   = TextureFilter_MinMagMip_Point;
	ssPointWrap.addressU = AddressMode_Wrap;
	ssPointWrap.addressV = AddressMode_Wrap;
	ssPointWrap.addressW = AddressMode_Wrap;

	SSPointWrap = createSamplerState(ssPointWrap);

	SamplerState ssPointClamp;
	ssPointClamp.filter = TextureFilter_MinMagMip_Point;

	SSPointClamp = createSamplerState(ssPointClamp);

	SamplerState ssLinearWrap;
	ssLinearWrap.addressU = AddressMode_Wrap;
	ssLinearWrap.addressV = AddressMode_Wrap;
	ssLinearWrap.addressW = AddressMode_Wrap;

	SSLinearWrap = createSamplerState(ssLinearWrap);

	SSLinearClamp = createSamplerState(SamplerState());

	SamplerState ssAnisotropicWrap;
	ssAnisotropicWrap.filter        = TextureFilter_Anisotropic;
	ssAnisotropicWrap.maxAnisotropy = maxAnisotropy;
	ssAnisotropicWrap.addressU      = AddressMode_Wrap;
	ssAnisotropicWrap.addressV      = AddressMode_Wrap;
	ssAnisotropicWrap.addressW      = AddressMode_Wrap;

	SSAnisotropicWrap = createSamplerState(ssAnisotropicWrap);

	SamplerState ssAnisotropicClamp;
	ssAnisotropicClamp.filter        = TextureFilter_Anisotropic;
	ssAnisotropicClamp.maxAnisotropy = maxAnisotropy;

	SSAnisotropicClamp = createSamplerState(ssAnisotropicClamp);

	// set defaults
	setBlendState(BSAlphaBlend);
	setDepthStencilState(DSSDefault);
	setRasterizerState(RSCullCounterClockwise);
	setPSSamplers(&SSLinearClamp, 0, 1);
}

void GraphicsContext::dispose()
{
	releaseBlendState(BSAlphaBlend);
	releaseBlendState(BSAdditive);
	//releaseBlendState(BSNonPremultiplied);
	releaseBlendState(BSOpaque);

	releaseDepthStencilState(DSSDefault);
	releaseDepthStencilState(DSSRead);
	releaseDepthStencilState(DSSNone);

	releaseRasterizerState(RSCullClockwise);
	releaseRasterizerState(RSCullCounterClockwise);
	releaseRasterizerState(RSCullNone);
	releaseRasterizerState(RSWireframe);

	releaseSamplerState(SSPointWrap);
	releaseSamplerState(SSPointClamp);
	releaseSamplerState(SSLinearWrap);
	releaseSamplerState(SSLinearClamp);
	releaseSamplerState(SSAnisotropicWrap);
	releaseSamplerState(SSAnisotropicClamp);
}
