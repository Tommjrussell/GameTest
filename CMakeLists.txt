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

Option (USE_INTELHD3000 "Enable to support this particular GPU" OFF)

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

add_subdirectory(salEngine)
add_subdirectory(AppTest)
add_subdirectory(salEngineTest)
