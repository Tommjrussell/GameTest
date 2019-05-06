#include "UI.h"

void MenuStateRenderer::Init()
{
	const auto viewIO = static_cast<SokolViewIO*>(GetViewIO());
	const auto menuState = static_cast<MenuState*>(GetState());

	auto& fc = *viewIO->GetFonts();

	float itemY = 100;
	for (size_t i = 0; i < menuState->m_menuItems.size(); i++)
	{
		auto& mi = menuState->m_menuItems[i];
		m_itemBBoxs.emplace_back(fc.GetStringBB(viewIO->GetMenuItemFont(), 50, itemY, true, mi.second.c_str()));
		itemY += 25;
	}
}

void MenuStateRenderer::Render()
{
	const auto viewIO = static_cast<SokolViewIO*>(GetViewIO());
	const auto menuState = static_cast<MenuState*>(GetState());

	auto& fc = *viewIO->GetFonts();

	fc.StartFrame();

	fc.DrawString(viewIO->GetMenuTitleFont(), 50, 50, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), true, menuState->m_title.c_str());

	float itemY = 100;

	for (size_t i = 0; i < menuState->m_menuItems.size(); i++)
	{
		auto& mi = menuState->m_menuItems[i];
		auto col = (i == m_selected) ? glm::vec4(0.8f, 1.0f, 1.0f, 1.0f) : glm::vec4(0.5f, 1.0f, 1.0f, 1.0f);
		fc.DrawString(viewIO->GetMenuItemFont(), 50, itemY, col, true, mi.second.c_str());
		itemY += 25;
	}

	sg_begin_default_pass(&viewIO->GetPassAction(), viewIO->GetWidth(), viewIO->GetHeight());
	fc.Render(viewIO->GetWidth(), viewIO->GetHeight());
	sg_end_pass();
}

void MenuStateRenderer::OnKeyboardInput(bool down, int keyCode)
{
	const auto menuState = static_cast<MenuState*>(GetState());
	const auto vIO = GetViewIO();

	if (!down)
		return;

	switch (keyCode)
	{
	case sal::SAL_KEYCODE_UP:
	{
		if (m_selected == ~0)
			m_selected = 0;
		else
			m_selected = (m_selected > 0) ? m_selected - 1 : menuState->m_menuItems.size() - 1;
	}
	break;
	case sal::SAL_KEYCODE_DOWN:
		m_selected = (m_selected + 1) % menuState->m_menuItems.size();
		break;
	case sal::SAL_KEYCODE_ENTER:
	{
		if (m_selected < menuState->m_menuItems.size())
		{
			sal::Message m(ui::kMessageOptionSelected);
			m.m_param1 = uint16_t(menuState->m_menuItems[m_selected].first);
			vIO->AddMessage(std::move(m));
		}
	}	break;

	}
}

void MenuStateRenderer::OnMouseInput(bool down, int mouseButtonCode)
{
	const auto menuState = static_cast<MenuState*>(GetState());

	if (down)
	{
		if (mouseButtonCode == sal::SAL_MOUSEBUTTON_LEFT)
		{
			if (m_selected < menuState->m_menuItems.size())
			{
				const auto vIO = GetViewIO();

				sal::Message m(ui::kMessageOptionSelected);
				m.m_param1 = uint16_t(menuState->m_menuItems[m_selected].first);
				vIO->AddMessage(std::move(m));
			}
		}
	}
}

void MenuStateRenderer::OnMouseMove(const sal::Point2D& mouseXY)
{
	const auto menuState = static_cast<MenuState*>(GetState());

	glm::vec2 p { mouseXY.x, mouseXY.y };

	for (size_t i = 0; i < menuState->m_menuItems.size(); i++)
	{
		if (m_itemBBoxs[i].Inside(p))
		{
			m_selected = i;
			return;
		}
	}
	m_selected = ~0;
}
