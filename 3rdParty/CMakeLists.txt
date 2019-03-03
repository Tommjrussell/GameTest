# glfw
set(GLFW_BUILD_EXAMPLES  OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS     OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS      OFF CACHE INTERNAL "")
set(GLFW_INSTALL         OFF CACHE INTERNAL "")
add_subdirectory(glfw)

add_subdirectory(glad)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_LIST_DIR}/glm)