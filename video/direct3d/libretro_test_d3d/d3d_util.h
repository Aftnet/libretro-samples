#ifndef _D3D_UTIL_H_
#define _D3D_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

struct ID3D11Device;

void d3d_util_init(ID3D11Device* device, int width, int height);
void* d3d_util_get_render_target();
void* d3d_util_get_depth_stencil_view();

#ifdef __cplusplus
}
#endif

#endif