#include "Driver.h"

#if defined(USE_SOKOL)

bool sal::SokolAppDriver::Run(const WindowDetails& details)
{
	stm_setup();

	sapp_desc desc = {};
	desc.user_data = (void*)this;
	desc.init_userdata_cb = [](void* thisPtr)
	{
		((SokolAppDriver*)thisPtr)->CallInit();
	};
	desc.frame_userdata_cb = [](void* thisPtr)
	{
		const auto ths = ((SokolAppDriver*)thisPtr);
		double lapTime = stm_sec(stm_laptime(&ths->m_lastTime));
		if (!ths->CallFrame(lapTime))
		{
			// TODO : wait for sapp to implement a way of quitting an app programmatically!
		}

		ths->SetWidthHeight(sapp_width(), sapp_height());

		ths->CallRender();
	};
	desc.event_userdata_cb = [](const sapp_event * event, void* thisPtr)
	{
		auto ths = ((SokolAppDriver*)thisPtr);

		switch (event->type)
		{
		case SAPP_EVENTTYPE_KEY_DOWN:
			ths->KeyboardInput(true, event->key_code);
			break;

		case SAPP_EVENTTYPE_KEY_UP:
			ths->KeyboardInput(false, event->key_code);
			break;

		case SAPP_EVENTTYPE_CHAR:
			ths->KeyCharacter(event->char_code, event->modifiers);
			break;

		case SAPP_EVENTTYPE_MOUSE_DOWN:
			ths->MouseInput(true, event->mouse_button);
			break;

		case SAPP_EVENTTYPE_MOUSE_UP:
			ths->MouseInput(true, event->mouse_button);
			break;

		case SAPP_EVENTTYPE_MOUSE_SCROLL:
			// TODO
			break;

		case SAPP_EVENTTYPE_MOUSE_MOVE:
			ths->MouseMove({ event->mouse_x, event->mouse_y });
			break;

		default:
			break;
		}
	};
	desc.cleanup_userdata_cb = [](void* thisPtr)
	{
		((SokolAppDriver*)thisPtr)->CallCleanup();
	};
	desc.fail_userdata_cb = [](const char*, void*)
	{
		// todo error reports
	};
	desc.width = details.width;
	desc.height = details.height;
	desc.window_title = details.title;
	sapp_run(&desc);

	return true;
}
#elif defined(USE_GLFW)

bool sal::GlfwAppDriver::Run(const WindowDetails& details)
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);

#if defined(USE_INTELHD3000)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	auto window = glfwCreateWindow(details.width, details.height, details.title, 0, 0);
	glfwMakeContextCurrent(window);

	glfwSetWindowUserPointer(window, (void*)this);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(1);

	glfwSetKeyCallback(window, [](GLFWwindow * window, int key, int scancode, int action, int mods)
		{
			auto ths = (GlfwAppDriver*)glfwGetWindowUserPointer(window);

			if (action != GLFW_PRESS && action != GLFW_RELEASE)
				return;

			ths->KeyboardInput(action == GLFW_PRESS, key);
		});

	glfwSetCharModsCallback(window, [](GLFWwindow * window, unsigned int codepoint, int mods)
		{
			auto ths = (GlfwAppDriver*)glfwGetWindowUserPointer(window);
			ths->KeyCharacter(codepoint, mods);
		});

	glfwSetMouseButtonCallback(window, [](GLFWwindow * window, int btn, int action, int mods)
		{
			auto ths = (GlfwAppDriver*)glfwGetWindowUserPointer(window);
			ths->MouseInput(action == GLFW_PRESS, btn);
		});

	glfwSetCursorPosCallback(window, [](GLFWwindow * window, double pos_x, double pos_y)
		{
			auto ths = (GlfwAppDriver*)glfwGetWindowUserPointer(window);
			ths->MouseMove({ float(pos_x), float(pos_y) });
		});

	glfwSetScrollCallback(window, [](GLFWwindow * w, double xOffset, double yOffset)
		{
			// TODO
		});

	CallInit();

	glfwSetTime(0.0);

	while (!glfwWindowShouldClose(window))
	{
		int windowWidth;
		int windowHeight;

		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		SetWidthHeight(windowWidth, windowHeight);

		auto time = glfwGetTime();
		glfwSetTime(0.0);

		if (!CallFrame(time))
		{
			break;
		}

		CallRender();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	CallCleanup();

	glfwDestroyWindow(window);
	glfwTerminate();

	return true;
}

#endif

#if defined(USE_SOKOL) || defined(USE_GLFW)

void sal::SokolBaseViewIO::Setup()
{
	sg_desc desc = {};
	sg_setup(&desc);
}

void sal::SokolBaseViewIO::OnRenderBegin()
{
}

void sal::SokolBaseViewIO::OnRenderEnd()
{
	sg_commit();
}

void sal::SokolBaseViewIO::Teardown()
{
}

#endif
