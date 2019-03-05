#include "glfwAppBackend.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"

void GlfwAppBackend::Init(const char* title, int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(width, height, title, 0, 0);
	glfwMakeContextCurrent(m_window);

	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glfwSwapInterval(1);
}

void GlfwAppBackend::Loop(std::function<void()>& frameFunc)
{
	while (!glfwWindowShouldClose(m_window))
	{
		frameFunc();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void GlfwAppBackend::Shutdown()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}