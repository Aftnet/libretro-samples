#include "renderer.h"

static unsigned int framebuffer_width = 0;
static unsigned int framebuffer_height = 0;

static sg_shader shd = { 0 };
static sg_pipeline pip = { 0 };
static sg_draw_state draw_state = { 0 };
static sg_pass_action pass_action = { 0 };

void renderer_init(ID3D11Device* device, ID3D11DeviceContext* context, unsigned int fb_width, unsigned int fb_height)
{
	framebuffer_width = fb_width;
	framebuffer_height = fb_height;

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
	sg_shader shd = sg_make_shader(&(sg_shader_desc) {
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
	sg_pipeline pip = sg_make_pipeline(&(sg_pipeline_desc) {
		.shader = shd,
			.layout = {
			.attrs = {
			[0] = { .name = "position",.format = SG_VERTEXFORMAT_FLOAT3 },
			[1] = { .name = "color0",.format = SG_VERTEXFORMAT_FLOAT4 }
		}
		}
	});

	/* a draw state with all the resource binding */
	sg_draw_state draw_state = {
		.pipeline = pip,
		.vertex_buffers[0] = vbuf
	};

	/* default pass action (clear to grey) */
	sg_pass_action pass_action = { 0 };

}

void renderer_deinit()
{
	sg_shutdown();
}

void renderer_render_frame()
{
	sg_begin_default_pass(&pass_action, framebuffer_width, framebuffer_height);
	sg_apply_draw_state(&draw_state);
	sg_draw(0, 3, 1);
	sg_end_pass();
	sg_commit();
}