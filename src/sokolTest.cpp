#include "glad/glad.h"
#include "sokol_gfx.h"
#include "sokol_app.h"

sg_pass_action pass_action = {0};
sg_buffer vbuf;
sg_shader shd;
sg_pipeline pip;
sg_bindings binds = {0};

const float vertices[] =
{
	// positions			// colors
	 0.0f,  0.5f, 0.5f,		1.0f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f, 1.0f
};

void init(void)
{
	{
		sg_desc args = { 0 };
		args.mtl_device = sapp_metal_get_device();
		args.mtl_renderpass_descriptor_cb = sapp_metal_get_renderpass_descriptor;
		args.mtl_drawable_cb = sapp_metal_get_drawable;
		args.d3d11_device = sapp_d3d11_get_device();
		args.d3d11_device_context = sapp_d3d11_get_device_context();
		args.d3d11_render_target_view_cb = sapp_d3d11_get_render_target_view;
		args.d3d11_depth_stencil_view_cb = sapp_d3d11_get_depth_stencil_view;
		sg_setup(&args);
	}

	{
		sg_buffer_desc args = { 0 };
		args.size = sizeof(vertices);
		args.content = vertices;
		vbuf = sg_make_buffer(&args);
	}

	{
		sg_shader_desc args = { 0 };
		args.vs.source =
			"#version 330\n"
			"in vec4 position;\n"
			"in vec4 color0;\n"
			"out vec4 color;\n"
			"void main() {\n"
			"  gl_Position = position;\n"
			"  color = color0;\n"
			"}\n";
		args.fs.source =
			"#version 330\n"
			"in vec4 color;\n"
			"out vec4 frag_color;\n"
			"void main() {\n"
			"  frag_color = color;\n"
			"}\n";
		shd = sg_make_shader(&args);
	}

	{
		sg_pipeline_desc args = { 0 };
		args.shader = shd;
		args.layout = { 0 };
		args.layout.attrs[0].name = "position";
		args.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
		args.layout.attrs[1].name = "color0";
		args.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;

		pip = sg_make_pipeline(&args);
	}

	binds.vertex_buffers[0] = vbuf;
}

void frame(void)
{
	sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
	sg_apply_pipeline(pip);
	sg_apply_bindings(&binds);
	sg_draw(0, 3, 1);
	sg_end_pass();
	sg_commit();
}

void cleanup(void)
{
	sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[])
{
	sapp_desc args = {0};
	args.init_cb = init;
	args.frame_cb = frame;
	args.cleanup_cb = cleanup;
	args.width = 400;
	args.height = 300;
	args.window_title = "Clear (sokol app)";
	return args;
}