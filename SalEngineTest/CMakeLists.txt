add_executable(SalEngineTest
	"salEngineTest.cpp")

target_link_libraries(SalEngineTest PUBLIC sal sokol imgui glm stb ${GameTest_LIBRARIES})
