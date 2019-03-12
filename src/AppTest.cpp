#include "App.h"

#define SOKOL_NO_DEPRECATED
#include "sokol_gfx.h"
#include "sokol_time.h"

#include "imgui.h"
#include "util/sokol_imgui.h"

#include "stb_image.h"

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include <vector>

struct vs_params_t
{
    ImVec2 disp_size;
};

struct ScreenQuadVertex
{
	glm::vec4 pos;
	glm::vec2 texcoord0;
};

struct SpriteVertex
{
	glm::vec3 pos;
	glm::i16vec2 texcoord0;
};

class MyApp : public tx0::App
{
private:
	virtual void Init(sg_desc args) override;
	virtual void Cleanup() override;
	virtual void Frame() override;
	virtual void Event(const sapp_event* event) override;

private:
	sg_image LoadTexture(const char* filename);

	void DrawSprite(int x, int y);

private:
	sg_pass_action m_passAction = { 0 };
	sg_shader m_shd;
	sg_pipeline m_pip;
	sg_bindings m_bind = { 0 };

	sg_pipeline m_imgui_pip;
	sg_bindings m_imgui_bind = { 0 };
	sg_shader m_imgui_shd;

	sg_pass m_offscreenPass;
	sg_pass_action m_offscreenPassAction;
	sg_pipeline m_offscreenPip;
	sg_bindings m_offscreenBind;

	uint64_t m_lastTime = 0;

	std::vector<SpriteVertex> m_spriteVertexList;
	std::vector<uint16_t> m_spriteIndexList;

	static const int kMaxImguiVertices = (1 << 17);
	static const int kMaxImguiIndices = kMaxImguiVertices * 3;

	bool m_buttonDown[SAPP_MAX_MOUSEBUTTONS];
	bool m_buttonUp[SAPP_MAX_MOUSEBUTTONS];

	struct Sprite
	{
		int x;
		int y;
	};
	std::vector<Sprite> m_sprites;
};

sg_image MyApp::LoadTexture(const char* filename)
{
	sg_image texture = {SG_INVALID_ID};
	int width, height, numChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &numChannels, 4);
	if (data == nullptr)
		return texture;

	sg_image_desc desc = {};
	desc.width = width;
	desc.height = height;
	desc.content.subimage[0][0].ptr = data;
	desc.content.subimage[0][0].size = width * height * 4;
	desc.label = "loaded texture";
	texture = sg_make_image(&desc);
	stbi_image_free(data);

	return texture;
}

void MyApp::Init(sg_desc args)
{
	sg_setup(&args);
	stm_setup();

	m_passAction.colors[0].action = SG_ACTION_CLEAR;
	m_passAction.colors[0].val[0] = 1.0f;
	m_passAction.colors[0].val[1] = 1.0f;
	m_passAction.colors[0].val[2] = 1.0f;
	m_passAction.colors[0].val[3] = 1.0f;

	simgui_desc_t desc{};
	simgui_setup(&desc);

	sg_image texture = LoadTexture("data\\SpriteSheet.png");

	// a render pass with one color image
	sg_image color_img;
	{
		{
			sg_image_desc img_desc = { 0 };
			img_desc.render_target = true;
			img_desc.width = 256;
			img_desc.height = 256;
			img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
			img_desc.min_filter = SG_FILTER_NEAREST;
			img_desc.mag_filter = SG_FILTER_NEAREST;
			img_desc.sample_count = 4;
			img_desc.label = "color-image";
			color_img = sg_make_image(&img_desc);
		}

		sg_pass_desc desc = { 0 };
		desc.color_attachments[0].image = color_img;
		desc.label = "offscreen-pass";

		m_offscreenPass = sg_make_pass(&desc);
	}

	// offscreen: clear to black
	m_offscreenPassAction = {};
    m_offscreenPassAction.colors[0].action = SG_ACTION_CLEAR;
	m_offscreenPassAction.colors[0].val[0] = 0.0f;
	m_offscreenPassAction.colors[0].val[1] = 1.0f;
	m_offscreenPassAction.colors[0].val[2] = 0.0f;
	m_offscreenPassAction.colors[0].val[3] = 1.0f;

	// a shader for a non-textured cube, rendered in the offscreen pass
	sg_shader offscreen_shd;
	{
		sg_shader_desc desc = {};
		desc.vs.uniform_blocks[0].size = sizeof(glm::mat4x4);
		desc.vs.uniform_blocks[0].uniforms[0].name = "mvp";
		desc.vs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
		desc.vs.source =
			R"#~#(
			#version 330
			uniform mat4 mvp;
			in vec3 position;
			in vec2 texcoord0;
			out vec2 uv;

			void main()
			{
				gl_Position = mvp * vec4(position, 1.0);
				uv = texcoord0;
			}
)#~#";
		desc.fs.source =
			R"#~#(
			#version 330
			uniform sampler2D tex;
			in vec2 uv;
			out vec4 frag_color;

			void main()
			{
				frag_color = texture(tex, uv);
			}

)#~#";
		desc.fs.images[0].name="tex";
		desc.fs.images[0].type=SG_IMAGETYPE_2D;
		desc.label = "offscreen-shader";
		offscreen_shd = sg_make_shader(&desc);
	}

	// pipeline-state-object for offscreen-rendered cube, don't need texture coord here
	{
		sg_pipeline_desc desc = {};
		desc.layout.attrs[0].name = "position";
		desc.layout.attrs[0].sem_name = "POSITION";
		desc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
		desc.layout.attrs[1].name = "texcoord0";
		desc.layout.attrs[1].sem_name = "TEXCOORD";
		desc.layout.attrs[1].format = SG_VERTEXFORMAT_SHORT2N;
		desc.shader = offscreen_shd;
		desc.index_type = SG_INDEXTYPE_UINT16;
		desc.blend.enabled = true;
		desc.blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
		desc.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
		desc.blend.color_write_mask = SG_COLORMASK_RGB;
		desc.blend.color_format = SG_PIXELFORMAT_RGBA8;
		desc.blend.depth_format = SG_PIXELFORMAT_NONE;
		desc.rasterizer.sample_count = 4;
		desc.label = "offscreen-pipeline";
		m_offscreenPip = sg_make_pipeline(&desc);
	}

	m_offscreenBind = {};

	m_offscreenBind.vertex_buffers[0] = tx0::NewBuffer()
		.Usage(SG_USAGE_STREAM)
		.Size(kMaxImguiVertices * sizeof(SpriteVertex))
		.Make();

	m_offscreenBind.index_buffer = tx0::NewBuffer()
		.Type(SG_BUFFERTYPE_INDEXBUFFER)
		.Usage(SG_USAGE_STREAM)
		.Size(kMaxImguiIndices * sizeof(uint16_t))
		.Make();

	m_offscreenBind.fs_images[0] = texture;

	const ScreenQuadVertex vertices[] =
	{
		{{-1.0f, 1.0f, 1.0f, 1.0f},	{0.0f, 0.0f}},
		{{ 1.0f, 1.0f, 1.0f, 1.0f},	{1.0f, 0.0f}},
		{{ 1.0f,-1.0f, 1.0f, 1.0f},	{1.0f, 1.0f}},
		{{-1.0f,-1.0f, 1.0f, 1.0f},	{0.0f, 1.0f}},
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

	m_bind.fs_images[0] = color_img;

	{
		sg_shader_desc args = { 0 };
		args.vs.source =
			R"#~#(
			#version 330

			in vec4 position;
			in vec2 texcoord0;
			out vec2 uv;

			void main()
			{
				gl_Position = position;
				uv = texcoord0;
			}

)#~#";

		args.fs.source =
			R"#~#(
			#version 330

			uniform sampler2D tex;
			in vec2 uv;
			out vec4 frag_color;

			void main()
			{
				frag_color = texture(tex, uv);
			}

)#~#";
		args.fs.images[0].name = "tex";
		args.fs.images[0].type = SG_IMAGETYPE_2D;
		m_shd = sg_make_shader(&args);
	}

	{
		sg_pipeline_desc args = { 0 };
		args.shader = m_shd;
		args.index_type = SG_INDEXTYPE_UINT16;
		//args.layout = { 0 };
		args.layout.attrs[0].name = "position";
		args.layout.attrs[0].sem_name = "POS";
		args.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT4;
		args.layout.attrs[1].name = "texcoord0";
		args.layout.attrs[1].sem_name = "TEXCOORD";
		args.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT2;
		args.label = "default-pipeline";
		m_pip = sg_make_pipeline(&args);
	}
}

void MyApp::Cleanup()
{
	simgui_shutdown();
	sg_shutdown();
}

void MyApp::DrawSprite(int x, int y)
{
	m_sprites.emplace_back(Sprite{x, y});
}

void MyApp::Frame()
{
	const auto width = GetWindowWidth();
	const auto height = GetWindowHeight();

	simgui_new_frame(width, height, stm_sec(stm_laptime(&m_lastTime)));

	ImGuiIO& io = ImGui::GetIO();
	for (int i = 0; i < SAPP_MAX_MOUSEBUTTONS; i++)
	{
		if (m_buttonDown[i])
		{
			m_buttonDown[i] = false;
			io.MouseDown[i] = true;
		}
		else if (m_buttonUp[i])
		{
			m_buttonUp[i] = false;
			io.MouseDown[i] = false;
		}
	}
	//if (io.WantTextInput && !sapp_keyboard_shown())
	//{
	//	sapp_show_keyboard(true);
 //   }
 //   if (!io.WantTextInput && sapp_keyboard_shown())
	//{
	//	sapp_show_keyboard(false);
	//}

	static bool show_test_window = true;
	static bool show_another_window = true;

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	static float f = 0.0f;
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", &m_passAction.colors[0].val[0]);
	if (ImGui::Button("Test Window")) show_test_window ^= 1;
	if (ImGui::Button("Another Window")) show_another_window ^= 1;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("btn_down: %d %d %d\n", m_buttonDown[0], m_buttonDown[1], m_buttonDown[2]);
	ImGui::Text("btn_up: %d %d %d\n", m_buttonUp[0], m_buttonUp[1], m_buttonUp[2]);

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(460, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow();
	}

	// Create the sprites
	m_sprites.clear();

	DrawSprite(128, 128);
	DrawSprite(20, 30);
	DrawSprite(40, 50);
	DrawSprite(220, 30);

	{
		m_spriteVertexList.clear();
		m_spriteIndexList.clear();
		const auto numSprites = m_sprites.size();
		m_spriteVertexList.resize(numSprites * 4);
		m_spriteIndexList.resize(numSprites * 6);

		for (size_t i = 0; i < numSprites; i++)
		{
			const auto& s = m_sprites[i];
			const auto vn = i*4;
			const auto in = i*6;

			m_spriteVertexList[vn+0].pos = glm::vec3(s.x - 8.0f, s.y - 8.0f, 0.0f);
			m_spriteVertexList[vn+0].texcoord0 = { 0, 0 };

			m_spriteVertexList[vn+1].pos = glm::vec3(s.x + 8.0f, s.y - 8.0f, 0.0f);
			m_spriteVertexList[vn+1].texcoord0 = { 0x800, 0 };

			m_spriteVertexList[vn+2].pos = glm::vec3(s.x - 8.0f, s.y + 8.0f, 0.0f);
			m_spriteVertexList[vn+2].texcoord0 = { 0, 0x800 };

			m_spriteVertexList[vn+3].pos = glm::vec3(s.x + 8.0f, s.y + 8.0f, 0.0f);
			m_spriteVertexList[vn+3].texcoord0 = { 0x800, 0x800 };

			m_spriteIndexList[in+0] = uint16_t(vn+0);
			m_spriteIndexList[in+1] = uint16_t(vn+1);
			m_spriteIndexList[in+2] = uint16_t(vn+2);

			m_spriteIndexList[in+3] = uint16_t(vn+1);
			m_spriteIndexList[in+4] = uint16_t(vn+3);
			m_spriteIndexList[in+5] = uint16_t(vn+2);
		}
	}

	// THE RENDERING!!!

	const auto view = glm::identity<glm::mat4>();
	const auto ortho = glm::ortho(0.0f, float(256), 0.0f, float(256), -1000.0f, 1000.0f);

	// the offscreen pass, rending some sprites

	sg_begin_pass(m_offscreenPass, &m_offscreenPassAction);
	m_offscreenBind.vertex_buffer_offsets[0] = sg_append_buffer(m_offscreenBind.vertex_buffers[0], m_spriteVertexList.data(), int(m_spriteVertexList.size() * sizeof(SpriteVertex)));
	m_offscreenBind.index_buffer_offset = sg_append_buffer(m_offscreenBind.index_buffer, m_spriteIndexList.data(), int(m_spriteIndexList.size() * sizeof(uint16_t)));
	sg_apply_pipeline(m_offscreenPip);
	sg_apply_bindings(&m_offscreenBind);
	sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &ortho, sizeof(ortho));
	sg_draw(0, m_spriteIndexList.size(), 1);
	sg_end_pass();

	sg_begin_default_pass(&m_passAction, width, height);
	sg_apply_pipeline(m_pip);
	sg_apply_bindings(&m_bind);
	sg_draw(0, 6, 1);

	simgui_render();

	sg_end_pass();
	sg_commit();
}

void MyApp::Event(const sapp_event* event)
{
	auto& io = ImGui::GetIO();
	io.KeyAlt = (event->modifiers & SAPP_MODIFIER_ALT) != 0;
	io.KeyCtrl = (event->modifiers & SAPP_MODIFIER_CTRL) != 0;
	io.KeyShift = (event->modifiers & SAPP_MODIFIER_SHIFT) != 0;
	io.KeySuper = (event->modifiers & SAPP_MODIFIER_SUPER) != 0;

	switch (event->type)
	{
	case SAPP_EVENTTYPE_MOUSE_DOWN:
		io.MousePos.x = event->mouse_x;
		io.MousePos.y = event->mouse_y;
		m_buttonDown[event->mouse_button] = true;
		break;
	case SAPP_EVENTTYPE_MOUSE_UP:
		io.MousePos.x = event->mouse_x;
		io.MousePos.y = event->mouse_y;
		m_buttonUp[event->mouse_button] = true;
		break;
	case SAPP_EVENTTYPE_MOUSE_MOVE:
		io.MousePos.x = event->mouse_x;
		io.MousePos.y = event->mouse_y;
		break;
	case SAPP_EVENTTYPE_MOUSE_SCROLL:
		io.MouseWheelH = event->scroll_x;
		io.MouseWheel = event->scroll_y;
		break;
	case SAPP_EVENTTYPE_KEY_DOWN:
		io.KeysDown[event->key_code] = true;
		break;
	case SAPP_EVENTTYPE_KEY_UP:
		io.KeysDown[event->key_code] = false;
		break;
	case SAPP_EVENTTYPE_CHAR:
		io.AddInputCharacter((ImWchar)event->char_code);
		break;
	}
}

int main()
{
	MyApp myApp;

	myApp.Run(640, 480, "My Window");

	return 0;
}