#ifndef _RENDERER_H_
#define _RENDERER_H_

struct retro_hw_render_interface_d3d11;

void renderer_init_d3d(struct retro_hw_render_interface_d3d11* d3d, int fb_width, int fb_height);
void renderer_deinit();
void renderer_render_frame();

#endif