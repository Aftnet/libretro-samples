#ifndef _D3D_UTIL_H_
#define _D3D_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

struct ID3D11Device;
struct ID3D11ShaderResourceView;

void d3d_util_init(ID3D11Device* device, int width, int height);
void d3d_util_deinit();
void* d3d_util_get_render_target();
ID3D11ShaderResourceView* d3d_util_get_render_target_shader_resource();
void* d3d_util_get_depth_stencil_view();

#ifdef __cplusplus
}
#endif

#endif