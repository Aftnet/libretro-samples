#pragma once

#include "libretro_d3d.h"


class Renderer
{
public:
	void Init();
	void Deinit();
	void RenderFrame();

private:
	ID3D11Device * device;
	ID3D11DeviceContext *context;
	D3D_FEATURE_LEVEL featureLevel;
	pD3DCompile D3DCompile;
};

