#ifndef _RENDERER_H_
#define _RENDERER_H_

struct retro_hw_render_interface_d3d11;

void renderer_setup_d3d(struct retro_hw_render_interface_d3d11* d3d);
void renderer_init();
void renderer_deinit();
void renderer_render_frame(int framebuffer_width, int framebuffer_height);

#endif