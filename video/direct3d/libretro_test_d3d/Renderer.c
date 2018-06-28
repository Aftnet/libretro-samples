#include "renderer.h"
#include "libretro_d3d.h"
#include "sokol_gfx.h"
#include "d3d_util.h"

static int framebuffer_width = 0;
static int framebuffer_height = 0;

static sg_shader shd = { 0 };
static sg_pipeline pip = { 0 };
static sg_draw_state draw_state = { 0 };
static sg_pass_action pass_action = { 0 };

void renderer_init();

void renderer_init_d3d(struct retro_hw_render_interface_d3d11* d3d, int fb_width, int fb_height)
{
	framebuffer_width = fb_width;
	framebuffer_height = fb_height;

	d3d_util_init(d3d->device, framebuffer_width, framebuffer_height);

	sg_desc desc = {
		.d3d11_device = d3d->device,
		.d3d11_device_context = d3d->context,
		.d3d11_render_target_view_cb = d3d_util_get_render_target,
		.d3d11_depth_stencil_view_cb = d3d_util_get_depth_stencil_view
	};

	sg_setup(&desc);
	renderer_init();
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
			"struct vs_in {\n"
			"  float4 pos: POS;\n"
			"  float4 color: COLOR;\n"
			"};\n"
			"struct vs_out {\n"
			"  float4 color: COLOR0;\n"
			"  float4 pos: SV_Position;\n"
			"};\n"
			"vs_out main(vs_in inp) {\n"
			"  vs_out outp;\n"
			"  outp.pos = inp.pos;\n"
			"  outp.color = inp.color;\n"
			"  return outp;\n"
			"}\n",
			.fs.source =
			"float4 main(float4 color: COLOR0): SV_Target0 {\n"
			"  return color;\n"
			"}\n"
	});

	/* a pipeline state object (default render states are fine for triangle) */
	pip = sg_make_pipeline(&(sg_pipeline_desc) {
		.shader = shd,
			.layout = {
			.attrs = {
			[0] = { .sem_name = "POS", .format = SG_VERTEXFORMAT_FLOAT3 },
			[1] = { .sem_name = "COLOR", .format = SG_VERTEXFORMAT_FLOAT4 }
		}}});

	/* a draw state with all the resource binding */
	draw_state.pipeline = pip;
	draw_state.vertex_buffers[0] = vbuf;
}

void renderer_deinit()
{
	sg_shutdown();
	d3d_util_deinit();
}

void renderer_render_frame()
{
	sg_begin_default_pass(&pass_action, framebuffer_width, framebuffer_height);
	sg_apply_draw_state(&draw_state);
	sg_draw(0, 3, 1);
	sg_end_pass();
	sg_commit();
}