#pragma once

#include "libretro_d3d.h"
#include "sokol_gfx.h"

void renderer_init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, unsigned int fbWidth, unsigned int fbHeight);
void renderer_deinit();
void renderer_render_frame();