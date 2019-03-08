#include "App.h"

#define SOKOL_NO_DEPRECATED
#include "sokol_gfx.h"

class MyApp : public tx0::App
{
private:
	virtual void Init(sg_desc args) override;
	virtual void Cleanup() override;
	virtual void Frame() override;

private:
	sg_pass_action m_passAction = { 0 };
	sg_shader m_shd;
	sg_pipeline m_pip;
	sg_bindings m_bind = { 0 };
};

void MyApp::Init(sg_desc args)
{
	sg_setup(&args);

	const float vertices[] =
	{
		// positions            colors
		-0.5f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 0.0f, 1.0f,
	};

	m_bind.vertex_buffers[0] = tx0::NewBuffer()
		.Size(sizeof vertices)
		.Content(vertices)
		.Make();

	const uint16_t indices[] = { 0, 1, 2,  0, 2, 3 };

	m_bind.index_buffer = tx0::NewBuffer()
		.Type(SG_BUFFERTYPE_INDEXBUFFER)
		.Size(sizeof(indices))
		.Content(indices)
		.Make();
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
		m_shd = sg_make_shader(&args);
	}

	{
		sg_pipeline_desc args = { 0 };
		args.shader = m_shd;
		args.index_type = SG_INDEXTYPE_UINT16;
		args.layout = { 0 };
		args.layout.attrs[0].name = "position";
		args.layout.attrs[0].sem_name = "POS";
		args.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
		args.layout.attrs[1].name = "color0";
		args.layout.attrs[0].sem_name = "COLOR";
		args.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;

		m_pip = sg_make_pipeline(&args);
	}
}

void MyApp::Cleanup()
{
	sg_shutdown();
}

void MyApp::Frame()
{
	sg_begin_default_pass(&m_passAction, GetWindowWidth(), GetWindowHeight());
	sg_apply_pipeline(m_pip);
	sg_apply_bindings(&m_bind);
	sg_draw(0, 6, 1);
	sg_end_pass();
	sg_commit();
}

int main()
{
	MyApp myApp;

	myApp.Run(640, 480, "My Window");

	return 0;
}