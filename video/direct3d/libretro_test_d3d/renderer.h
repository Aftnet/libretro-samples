#pragma once

#include "libretro_d3d.h"
#include "sokol_gfx.h"

class Renderer
{
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, unsigned int fbWidth, unsigned int fbHeight);
	void Deinit();
	void RenderFrame();

private:
	unsigned int FbWidth;
	unsigned int FbHeight;

	sg_shader shd;
	sg_pipeline pip;
	sg_draw_state draw_state;
	sg_pass_action pass_action;
};

