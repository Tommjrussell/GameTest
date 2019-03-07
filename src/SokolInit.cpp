//#define USE_GLFW
#define USE_SAPP

#define SOKOL_IMPL
#define SOKOL_NO_DEPRECATED
#define SOKOL_GLCORE33

#ifdef USE_GLFW
#	include "glad/glad.h"
#	include "GLFW/glfw3.h"
#elif defined(USE_SAPP)
#define SOKOL_WIN32_FORCE_MAIN
#define SOKOL_NO_ENTRY
#include "sokol_app.h"
#endif

#include "sokol_gfx.h"


