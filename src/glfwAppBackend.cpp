#include "Config.h"
#ifdef USE_GLFW

#include "glfwAppBackend.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "sokol_gfx.h"

namespace
{
	GlfwAppBackend* gCurrentInstance = nullptr;
}

bool GlfwAppBackend::Run(int width, int height, const char* title)
{
	_ASSERTE(gCurrentInstance == nullptr);
	gCurrentInstance = this;

	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	auto window = glfwCreateWindow(width, height, title, 0, 0);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(1);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (action)
		{
		case GLFW_RELEASE:
			gCurrentInstance->m_event.type = SAPP_EVENTTYPE_KEY_UP;
			break;
		case GLFW_PRESS:
			gCurrentInstance->m_event.type = SAPP_EVENTTYPE_KEY_DOWN;
			break;
		case GLFW_REPEAT:
			return; // TODO : how do we handle this?
		}
		gCurrentInstance->m_event.key_code = sapp_keycode(key); // Same codes in GLFW as SAPP
		gCurrentInstance->m_event.modifiers = mods; // Same Ids used by GLFW & SAPP
		gCurrentInstance->CallEventFunction(&gCurrentInstance->m_event);
	});

	glfwSetCharModsCallback(window, [](GLFWwindow* window, unsigned int codepoint, int mods)
	{
		gCurrentInstance->m_event.type = SAPP_EVENTTYPE_CHAR;
		gCurrentInstance->m_event.char_code = codepoint;
		gCurrentInstance->m_event.modifiers = mods; // Same Ids used by GLFW & SAPP
		gCurrentInstance->CallEventFunction(&gCurrentInstance->m_event);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int btn, int action, int mods)
	{
		if (btn < 0 && btn >= SAPP_MAX_MOUSEBUTTONS)
			return;

		gCurrentInstance->m_event.type = (action == GLFW_PRESS) ? SAPP_EVENTTYPE_MOUSE_DOWN : SAPP_EVENTTYPE_MOUSE_UP;
		gCurrentInstance->m_event.mouse_button = sapp_mousebutton(btn); // luckily, GLFW & SAPP use same ids for mouse buttons.
		gCurrentInstance->m_event.modifiers = mods; // Same Ids used by GLFW & SAPP
		gCurrentInstance->CallEventFunction(&gCurrentInstance->m_event);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* w, double pos_x, double pos_y)
	{
		gCurrentInstance->m_event.type = SAPP_EVENTTYPE_MOUSE_MOVE;
		gCurrentInstance->m_event.mouse_x = float(pos_x);
		gCurrentInstance->m_event.mouse_y = float(pos_y);
		gCurrentInstance->CallEventFunction(&gCurrentInstance->m_event);
	});

	glfwSetScrollCallback(window, [](GLFWwindow* w, double xOffset, double yOffset)
	{
		gCurrentInstance->m_event.type = SAPP_EVENTTYPE_MOUSE_SCROLL;
		gCurrentInstance->m_event.scroll_x = float(xOffset);
		gCurrentInstance->m_event.scroll_y = float(yOffset);
		gCurrentInstance->CallEventFunction(&gCurrentInstance->m_event);
	});

	CallInitFunction();

	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &m_windowWidth, &m_windowHeight);

		CallFrameFunction();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	CallShutdownFunction();

	glfwDestroyWindow(window);
	glfwTerminate();

	return true;
}

sg_desc GlfwAppBackend::GetSokolDesc() const
{
	sg_desc desc = { 0 };
	return desc;
}

#endif