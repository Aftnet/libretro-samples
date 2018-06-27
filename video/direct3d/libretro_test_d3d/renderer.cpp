#include "Renderer.h"

void Renderer::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, unsigned int fbWidth, unsigned int fbHeight)
{
	FbWidth = fbWidth;
	FbHeight = fbHeight;

	/* setup sokol_gfx */
	sg_desc sg_setup_desc = {};
	sg_setup_desc.d3d11_device = pDevice;
	sg_setup_desc.d3d11_device_context = pContext;
	sg_setup(&sg_setup_desc);

	/* a vertex buffer */
	static const float vertices[] = {
		// positions            // colors
		0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f
	};

	sg_buffer_desc vbuf_desc = {};
	vbuf_desc.size = sizeof(vertices);
	vbuf_desc.content = vertices;
	sg_buffer vbuf = sg_make_buffer(&vbuf_desc);

	/* a shader */
	sg_shader_desc shd_desc = {};
	shd_desc.vs.source =
		"#version 330\n"
		"in vec4 position;\n"
		"in vec4 color0;\n"
		"out vec4 color;\n"
		"void main() {\n"
		"  gl_Position = position;\n"
		"  color = color0;\n"
		"}\n";
	shd_desc.fs.source =
		"#version 330\n"
		"in vec4 color;\n"
		"out vec4 frag_color;\n"
		"void main() {\n"
		"  frag_color = color;\n"
		"}\n";
	sg_shader shd = sg_make_shader(&shd_desc);

	/* a pipeline state object (default render states are fine for triangle) */
	sg_pipeline_desc pip_desc = {};
	pip_desc.shader = shd;
	pip_desc.layout.attrs[0].name = "position";
	pip_desc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
	pip_desc.layout.attrs[1].name = "color0";
	pip_desc.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;
	sg_pipeline pip = sg_make_pipeline(&pip_desc);

	/* a draw state with all the resource binding */
	sg_draw_state draw_state = {};
	draw_state.pipeline = pip;
	draw_state.vertex_buffers[0] = vbuf;

	/* default pass action (clear to grey) */
	sg_pass_action pass_action = { 0 };
}

void Renderer::Deinit()
{
	sg_shutdown();
}

void Renderer::RenderFrame()
{
	int cur_width, cur_height;
	sg_begin_default_pass(&pass_action, FbWidth, FbHeight);
	sg_apply_draw_state(&draw_state);
	sg_draw(0, 3, 1);
	sg_end_pass();
	sg_commit();
}