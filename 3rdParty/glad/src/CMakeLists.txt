set(glad_HEADERS "${GLAD_SOURCE_DIR}/include/glad/glad.h" "${GLAD_SOURCE_DIR}/include/KHR/khrplatform.h")
set(glad_SOURCES "${GLAD_SOURCE_DIR}/src/glad.c")

add_library(glad ${glad_SOURCES} ${glad_HEADERS})

target_include_directories(glad PUBLIC
                           "$<BUILD_INTERFACE:${GLAD_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>")
