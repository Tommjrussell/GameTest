add_library(sal STATIC)

target_sources(sal INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include/sal/Engine.h
	${CMAKE_CURRENT_LIST_DIR}/include/sal/Driver.h
	${CMAKE_CURRENT_LIST_DIR}/include/sal/Gfx.h
	${CMAKE_CURRENT_LIST_DIR}/include/sal/Text.h
	${CMAKE_CURRENT_LIST_DIR}/include/sal/UI.h)

target_sources(sal PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/src/Engine.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/Driver.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/sokolImpl.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/Text.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/UI.cpp)

target_link_libraries(sal PUBLIC sokol imgui glm stb ${GameTest_LIBRARIES})

target_include_directories(sal PUBLIC include/sal)

set_target_properties(sal PROPERTIES FOLDER "sal")