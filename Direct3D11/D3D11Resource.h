#pragma once

#include <d3d11.h>

struct D3D11Resource
{
	ID3D11Resource *resource;
	ID3D11ShaderResourceView *srv;
};

