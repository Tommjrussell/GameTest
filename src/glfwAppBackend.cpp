#include "glfwAppBackend.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "sokol_gfx.h"

//#define USE_GLFW

#ifdef USE_GLFW

bool GlfwAppBackend::Run(int width, int height, const char* title)
{
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