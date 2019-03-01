# CMakeList.txt : CMake project for GameTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(GameTest)

set (GameTest_VERSION_MAJOR 1)
set (GameTest_VERSION_MINOR 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
  )

include_directories("${PROJECT_BINARY_DIR}")

# Add source to this project's executable.
add_executable (GameTest "GameTest.cpp" "GameTest.h")

# TODO: Add tests and install targets if needed.
