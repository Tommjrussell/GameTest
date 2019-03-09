#include "config.h"

#define SOKOL_IMPL
#define SOKOL_NO_DEPRECATED

#if (defined USE_OPENGL)
#	define SOKOL_GLCORE33
#elif (defined USE_DIRECTX)
#	define SOKOL_D3D11
#elif (defined USE_METAL)
#	define SOKOL_METAL
#endif

#ifdef USE_GLFW
#	include "glad/glad.h"
#	include "GLFW/glfw3.h"
#elif defined(USE_SAPP)
#define SOKOL_WIN32_FORCE_MAIN
#define SOKOL_NO_ENTRY
#include "sokol_app.h"
#endif

#include "sokol_gfx.h"


