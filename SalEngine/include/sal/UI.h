#pragma once

#include "Text.h"

#include "Engine.h"
#include "Driver.h"
#include <string>

namespace ui
{
	const int kMessageOptionSelected = 1024;
};

class SokolViewIO : public sal::SokolBaseViewIO
{
public:
	sal::FontCollection* GetFonts() { return m_fontCollection.get(); }
	const sg_pass_action& GetPassAction() { return m_passAction; }

	sal::FontHandle GetMenuTitleFont() const { return m_menuTitleFont; }
	sal::FontHandle GetMenuItemFont() const { return m_menuItemFont; }

protected:
	std::unique_ptr<sal::FontCollection> m_fontCollection;
	sg_pass_action m_passAction = {};

	sal::FontHandle m_menuTitleFont = {};
	sal::FontHandle m_menuItemFont = {};
};

class MenuState : public sal::State
{
	friend class MenuStateRenderer;

public:
	using sal::State::State;
	virtual bool IsOpaque() const { return true; }

	virtual void Update(double elapsedTime) override {}

protected:
	void SetTitle(const std::string& title) { m_title = title; }
	void AddMenuItem(int id, const std::string& label)
	{
		m_menuItems.emplace_back(id, label);
	}

private:
	typedef std::pair<int, std::string> MenuItem;

	std::string m_title;
	std::vector<MenuItem> m_menuItems;
};

class MenuStateRenderer : public sal::StateRenderer, public sal::ConcreteStateRenderer<MenuStateRenderer>
{
public:
	using sal::StateRenderer::StateRenderer;

	virtual void Init() override;

	virtual void Render() override;

	virtual void OnKeyboardInput(bool down, int keyCode) override;
	virtual void OnMouseInput(bool down, int mouseButtonCode) override;
	virtual void OnMouseMove(const sal::Point2D& mouseXY) override;

private:
	size_t m_selected = ~0;
	std::vector<sal::BBox> m_itemBBoxs;
};

