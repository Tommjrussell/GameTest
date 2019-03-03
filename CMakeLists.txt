# CMakeList.txt : CMake project for GameTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(GameTest)

find_package(OpenGL REQUIRED)

add_subdirectory(3rdParty)

set (GameTest_VERSION_MAJOR 1)
set (GameTest_VERSION_MINOR 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
  )

include_directories("${PROJECT_BINARY_DIR}" "${GLFW_SOURCE_DIR}/deps")

# Add source to this project's executable.
add_executable (GameTest "GameTest.cpp" "GameTest.h")

target_link_libraries(GameTest PUBLIC glfw glad glm OpenGL::GL)

# TODO: Add tests and install targets if needed.
