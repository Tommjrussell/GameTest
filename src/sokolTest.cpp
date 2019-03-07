#define SOKOL_NO_DEPRECATED
#include "sokol_gfx.h"
#include "sokol_app.h"

sg_pass_action pass_action = {0};
sg_shader shd;
sg_pipeline pip;
sg_bindings bind = {0};

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

	const float vertices[] =
	{
		// positions            colors
		-0.5f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 0.0f, 1.0f,
	};

	{
		sg_buffer_desc args = {0};
		args.size = sizeof(vertices);
		args.content = vertices;
		bind.vertex_buffers[0] = sg_make_buffer(&args);
	}

    const uint16_t indices[] = { 0, 1, 2,  0, 2, 3 };

	{
		sg_buffer_desc args = { 0 };
		args.type = SG_BUFFERTYPE_INDEXBUFFER;
		args.size = sizeof(indices);
		args.content = indices;
		bind.index_buffer = sg_make_buffer(&args);
	}

	{
		sg_shader_desc args = { 0 };
		args.vs.source =
R"#~#(
			#version 330

			in vec4 position;
			in vec4 color0;
			out vec4 color;

			void main()
			{
				gl_Position = position;
				color = color0;
			}

)#~#";

		args.fs.source =
R"#~#(
			#version 330

			in vec4 color;
			out vec4 frag_color;

			void main()
			{
				frag_color = color;
			}

)#~#";
		shd = sg_make_shader(&args);
	}

	{
		sg_pipeline_desc args = { 0 };
		args.shader = shd;
		args.index_type = SG_INDEXTYPE_UINT16;
		args.layout = { 0 };
		args.layout.attrs[0].name = "position";
		args.layout.attrs[0].sem_name = "POS";
		args.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
		args.layout.attrs[1].name = "color0";
		args.layout.attrs[0].sem_name = "COLOR";
		args.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;

		pip = sg_make_pipeline(&args);
	}
}

void frame(void)
{
	sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
	sg_apply_pipeline(pip);
	sg_apply_bindings(&bind);
	sg_draw(0, 6, 1);
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