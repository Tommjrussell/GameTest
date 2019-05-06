add_executable(AppTest
	"AppTest.cpp"
	"App.h"				"App.cpp"
	"AppBackend.h"		"AppBackend.cpp"
	"glfwAppBackend.h"	"glfwAppBackend.cpp"
	"sappAppBackend.h"	"sappAppBackend.cpp"
	"AppImgui.h"		"AppImgui.cpp"
	"SokolInit.cpp")

target_link_libraries(AppTest PUBLIC sokol imgui glm stb ${GameTest_LIBRARIES})

add_custom_command(TARGET AppTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data)

add_custom_command(
		TARGET AppTest POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${CMAKE_SOURCE_DIR}/AppTest/data/SpriteSheet.png
				${CMAKE_CURRENT_BINARY_DIR}/data/SpriteSheet.png)