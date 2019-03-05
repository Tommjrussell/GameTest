#pragma once

#include "AppBackend.h"

struct GLFWwindow;

class GlfwAppBackend : public AppBackend
{
public:
	virtual void Init(const char* title, int width, int height) override;
	virtual void Loop(std::function<void()>& frameFunc) override;
	virtual void Shutdown() override;

private:
	GLFWwindow* m_window;
};
