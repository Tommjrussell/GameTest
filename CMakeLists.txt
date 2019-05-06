# CMakeList.txt : CMake project for GameTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(GameTest)

find_package(OpenGL REQUIRED)

add_subdirectory(3rdParty)

set (GameTest_VERSION_MAJOR 1)
set (GameTest_VERSION_MINOR 0)

Option (USE_GLFW "Use GLFW as the framework" ON)
Option (USE_SAPP "Use SAPP as the framework" OFF)

Option (USE_OPENGL "Use OpenGL 3.3 as the API" ON)
Option (USE_DIRECTX "Use DX11 as the API" OFF)
Option (USE_METAL "Use Metal as the API" OFF)

if (USE_GLFW)
	list(APPEND GameTest_LIBRARIES glfw glad)
elseif (USE_SAPP)
endif()

if (USE_OPENGL)
	list(APPEND GameTest_LIBRARIES OpenGL::GL)
elseif (USE_DIRECTX)
elseif (USE_METAL)
endif()

configure_file (
	"${PROJECT_SOURCE_DIR}/Config.h.in"
	"${PROJECT_BINARY_DIR}/Config.h")

include_directories("${PROJECT_BINARY_DIR}")

add_executable(AppTest
	"src/AppTest.cpp"
	"src/App.h"				"src/App.cpp"
	"src/AppBackend.h"		"src/AppBackend.cpp"
	"src/glfwAppBackend.h"	"src/glfwAppBackend.cpp"
	"src/sappAppBackend.h"	"src/sappAppBackend.cpp"
	"src/AppImgui.h"		"src/AppImgui.cpp"
	"src/SokolInit.cpp")
target_link_libraries(AppTest PUBLIC sokol imgui glm stb ${GameTest_LIBRARIES})

add_custom_command(TARGET AppTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data)

add_custom_command(
		TARGET AppTest POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${CMAKE_SOURCE_DIR}/data/SpriteSheet.png
				${CMAKE_CURRENT_BINARY_DIR}/data/SpriteSheet.png)