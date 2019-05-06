#include "Engine.h"
#include "Driver.h"

#include "UI.h"
#include "Text.h"

#include "glm/glm.hpp"
#include <memory>

enum MyMessages : uint16_t
{
	ChangeState = 1,
};

struct ScreenQuadVertex
{
	glm::vec3 pos;
	glm::vec4 colour;
};

class MyApp : public sal::App
{
   public:
	explicit MyApp(std::unique_ptr<sal::AppDriver>&& driver);

	void OnStartup() override;
	void OnShutdown() override {};
};

MyApp::MyApp(std::unique_ptr<sal::AppDriver>&& driver) : App(std::move(driver)) {}

constexpr int kMyMainMenuId = 2;
constexpr int kMyStateId = 3;
constexpr int kMyOtherStateId = 4;


class MyState : public sal::ConcreteState<MyState, sal::State, kMyStateId>
{
public:
	MyState(sal::App& app, State* parent) : ParentClass(app, parent) {}

	virtual void Update(double elapsedTime) override {}

	bool HandleMessage(sal::Message& m) override
	{
		if (m.m_type != MyMessages::ChangeState)
			return false;

		ChangeState(kMyOtherStateId);
		return true;
	}
};

class MyState2 : public sal::ConcreteState<MyState2, sal::State, kMyOtherStateId>
{
public:
	MyState2(sal::App& app, State* parent) : ParentClass(app, parent) {}

	virtual void Update(double elapsedTime) override {}

	bool HandleMessage(sal::Message& m) override
	{
		if (m.m_type != MyMessages::ChangeState)
			return false;

		ChangeState(kMyStateId);
		return true;
	}
};

class MyMenuState : public sal::ConcreteState<MyMenuState, MenuState, kMyMainMenuId>
{
public:
	MyMenuState(sal::App& app, State* parent) : ParentClass(app, parent) {}

	void Init() override
	{
		SetTitle("My Menu!");
		AddMenuItem(1, "Start the Game!");
		AddMenuItem(2, "Is");
		AddMenuItem(3, "QUIT!");
	}

	bool HandleMessage(sal::Message& m) override
	{
		if (m.m_type == ui::kMessageOptionSelected)
		{
			switch (m.m_param1)
			{
			case 1:
				ChangeState(kMyStateId);
				return true;

			case 3:
				ChangeState(sal::kQuitStateId);
				return true;
			}
		}
		return false;
	}
};

class MyViewIO : public SokolViewIO
{
public:
	MyViewIO() = default;
private:

	void OnStartup() override
	{
		m_passAction.colors[0].action = SG_ACTION_CLEAR;
		m_passAction.colors[0].val[0] = 0.7f;
		m_passAction.colors[0].val[1] = 0.7f;
		m_passAction.colors[0].val[2] = 0.7f;
		m_passAction.colors[0].val[3] = 1.0f;

		sal::FontCollectionBuilder fcb(1024, 1024);
		const auto arial = fcb.AddTypeface("C:/Windows/Fonts/arial.ttf");
		const auto comic = fcb.AddTypeface("C:/Windows/Fonts/comic.ttf");
		m_bigArial = fcb.AddFont(arial, 32, 32, 128 - 32);
		m_littleArial = fcb.AddFont(arial, 12, 32, 128 - 32);
		m_bigComic = fcb.AddFont(comic, 48, 32, 128 - 32);
		m_fontCollection = fcb.Generate();

		m_menuTitleFont = m_bigArial;
		m_menuItemFont = m_littleArial;
	}

public:
	sal::FontHandle m_bigArial;
	sal::FontHandle m_littleArial;
	sal::FontHandle m_bigComic;
};

class MyStateRenderer : public sal::StateRenderer, public sal::ConcreteStateRenderer<MyStateRenderer>
{
public:
	using sal::StateRenderer::StateRenderer;

private:
	virtual void Init() override
	{
		const ScreenQuadVertex vertices[] = {
			{{-0.9f,  0.9f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},
			{{ 0.9f,  0.9f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},
			{{ 0.9f, -0.9f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},
			{{-0.9f, -0.9f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},
		};

		m_bind.vertex_buffers[0] = m_vertexBuffer = sal::NewBuffer()
			.Size(sizeof vertices)
			.Content(vertices)
			.Make();

		const uint16_t indices[] = { 0,1,2, 0,2,3 };

		m_bind.index_buffer = m_indexBuffer = sal::NewBuffer()
			.Type(SG_BUFFERTYPE_INDEXBUFFER)
			.Size(sizeof indices)
			.Content(indices)
			.Make();

		{
			sg_shader_desc desc = {0};
			desc.vs.source = "#version 330\n"
				"in vec4 position;\n"
				"in vec4 color0;\n"
				"out vec4 color;\n"
				"void main() {\n"
				"  gl_Position = position;\n"
				"  color = color0;\n"
				"}\n";
			desc.fs.source = "#version 330\n"
				"in vec4 color;\n"
				"out vec4 frag_color;\n"
				"void main() {\n"
				"  frag_color = color;\n"
				"}\n";
			desc.label = "triangle-shader";
			m_shader = sg_make_shader(&desc);
		}

		{
			sg_pipeline_desc desc = {};
			desc.shader = m_shader;
			desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
			desc.index_type = SG_INDEXTYPE_UINT16;
			desc.layout.attrs[0].name = "position";
			desc.layout.attrs[0].sem_name = "POS";
			desc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
			desc.layout.attrs[1].name = "color0";
			desc.layout.attrs[1].sem_name = "COLOR";
			desc.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;
			desc.label = "triangle-pipeline";
			m_pip = sg_make_pipeline(&desc);
		}
	}

	virtual void Render() override
	{
		auto viewIO = static_cast<MyViewIO*>(GetViewIO());

		auto fc = viewIO->GetFonts();

		fc->StartFrame();
		fc->DrawString(viewIO->m_bigArial, 50.0f, 100.0f, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), true, "The.");
		fc->DrawString(viewIO->m_littleArial, 50.0f, 200.0f, glm::vec4(1.0f, 0.2f, 0.5f, 1.0f), true, "Rvain!");
		fc->DrawString(viewIO->m_littleArial, 50.0f, 240.0f, glm::vec4(1.0f, 0.2f, 0.5f, 1.0f), true, "T'#@!");
		fc->DrawString(viewIO->m_bigComic, 50.0f, 320.0f, glm::vec4(0.4f, 0.6f, 0.5f, 1.0f), true, "C");

		sg_begin_default_pass(&viewIO->GetPassAction(), viewIO->GetWidth(), viewIO->GetHeight());
		sg_apply_pipeline(m_pip);
		sg_apply_bindings(&m_bind);
		sg_draw(0, 6, 1);

		fc->Render(viewIO->GetWidth(), viewIO->GetHeight());

		sg_end_pass();
	}

	void OnKeyboardInput(bool down, int keyCode) override
	{
		const auto vIO = GetViewIO();

		if (down && keyCode == sal::SAL_KEYCODE_ENTER)
		{
			vIO->AddMessage(sal::Message(ChangeState));
		}
	}

	sal::PipelineHandle m_pip;
	sal::ShaderHandle m_shader;
	sal::BufferHandle m_vertexBuffer;
	sal::BufferHandle m_indexBuffer;
	sg_bindings m_bind = {};
};

class MyStateRenderer2 : public sal::StateRenderer, public sal::ConcreteStateRenderer<MyStateRenderer2>
{
public:
	using sal::StateRenderer::StateRenderer;

private:
	virtual void Init() override
	{
	}

	virtual void Render() override
	{
		auto viewIO = static_cast<MyViewIO*>(GetViewIO());

		auto fc = viewIO->GetFonts();

		fc->StartFrame();
		fc->DrawString(viewIO->m_bigArial, 50.0f, 100.0f, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), true, "The quick brown fox jumps over the lazy dog.");
		fc->DrawString(viewIO->m_littleArial, 50.0f, 200.0f, glm::vec4(1.0f, 0.2f, 0.5f, 1.0f), true, "Richard of York gave battle in vain!");
		fc->DrawString(viewIO->m_littleArial, 50.0f, 240.0f, glm::vec4(1.0f, 0.2f, 0.5f, 1.0f), true, "Top stuff. I like this font malarky!?!:'#@!");
		fc->DrawString(viewIO->m_bigComic, 50.0f, 320.0f, glm::vec4(0.4f, 0.6f, 0.5f, 1.0f), true, "Comic Sans Baby! Yeah!!!");

		sg_begin_default_pass(&viewIO->GetPassAction(), viewIO->GetWidth(), viewIO->GetHeight());
		fc->Render(viewIO->GetWidth(), viewIO->GetHeight());
		sg_end_pass();

	}

	virtual void OnKeyboardInput(bool down, int keyCode) override
	{
		const auto vIO = GetViewIO();

		if (down && keyCode == sal::SAL_KEYCODE_ENTER)
		{
			vIO->AddMessage(sal::Message(ChangeState));
		}
	}
};

void MyApp::OnStartup()
{
	RegisterState(kMyMainMenuId, sal::kMasterStateId, MyMenuState::Create, MenuStateRenderer::Create);
	RegisterState(kMyStateId, sal::kMasterStateId, MyState::Create, MyStateRenderer::Create);
	RegisterState(kMyOtherStateId, sal::kMasterStateId, MyState2::Create, MyStateRenderer2::Create);
}

int main()
{
	sal::WindowDetails details;
	details.title = "SAL Window";
	details.width = 960;
	details.height = 540;

#if defined(USE_SOKOL)
	MyApp myApp(std::make_unique<sal::SokolAppDriver>(std::make_unique<MyViewIO>()));
#elif defined(USE_GLFW)
	MyApp myApp(std::make_unique<sal::GlfwAppDriver>(std::make_unique<MyViewIO>()));
#endif
	return myApp.Run(details);
}