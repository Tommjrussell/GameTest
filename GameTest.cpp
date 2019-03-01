#include "GameTest.h"
#include "glfw/glfw3.h"

int main()
{
	glfwInit();

	std::cout << "Version " << GameTest_VERSION_MAJOR << "." << GameTest_VERSION_MINOR << "\n";
	std::cout << "Hello CMake." << std::endl;

	glfwTerminate();

	return 0;
}
