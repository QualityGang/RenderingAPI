#pragma once

#include <d3d11.h>
#include <stdint.h>

struct D3D11RenderTarget
{
	uint32_t numRenderTargets;
	ID3D11RenderTargetView *renderTargetViews[8];
	ID3D11DepthStencilView *depthStencilView;
};