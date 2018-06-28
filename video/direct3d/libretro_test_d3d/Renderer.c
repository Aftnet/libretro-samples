#include "renderer.h"
#include "libretro_d3d.h"
#include "sokol_gfx.h"
#include "d3d_util.h"

static sg_shader shd = { 0 };
static sg_pipeline pip = { 0 };
static sg_draw_state draw_state = { 0 };
static sg_pass_action pass_action = { 0 };

void renderer_setup_d3d(struct retro_hw_render_interface_d3d11* d3d)
{
	sg_desc desc = {
		.d3d11_device = d3d->device,
		.d3d11_device_context = d3d->context,
		.d3d11_render_target_view_cb = d3d_util_get_render_target,
		.d3d11_depth_stencil_view_cb = d3d_util_get_depth_stencil_view
	};

	sg_setup(&desc);
}

void renderer_init()
{
	/* a vertex buffer */
	const float vertices[] = {
		// positions            // colors
		0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f
	};
	sg_buffer vbuf = sg_make_buffer(&(sg_buffer_desc) {
		.size = sizeof(vertices),
			.content = vertices,
	});

	/* a shader */
	shd = sg_make_shader(&(sg_shader_desc) {
		.vs.source =
			"#version 330\n"
			"in vec4 position;\n"
			"in vec4 color0;\n"
			"out vec4 color;\n"
			"void main() {\n"
			"  gl_Position = position;\n"
			"  color = color0;\n"
			"}\n",
			.fs.source =
			"#version 330\n"
			"in vec4 color;\n"
			"out vec4 frag_color;\n"
			"void main() {\n"
			"  frag_color = color;\n"
			"}\n"
	});

	/* a pipeline state object (default render states are fine for triangle) */
	pip = sg_make_pipeline(&(sg_pipeline_desc) {
		.shader = shd,
			.layout = {
			.attrs = {
			[0] = { .name = "position",.format = SG_VERTEXFORMAT_FLOAT3 },
			[1] = { .name = "color0",.format = SG_VERTEXFORMAT_FLOAT4 }
		}}});

	/* a draw state with all the resource binding */
	draw_state.pipeline = pip;
	draw_state.vertex_buffers[0] = vbuf;
}

void renderer_deinit()
{
	sg_shutdown();
}

void renderer_render_frame(int framebuffer_width, int framebuffer_height)
{
	sg_begin_default_pass(&pass_action, framebuffer_width, framebuffer_height);
	sg_apply_draw_state(&draw_state);
	sg_draw(0, 3, 1);
	sg_end_pass();
	sg_commit();
}