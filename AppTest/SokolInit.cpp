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
#	undef SOKOL_IMPL
#	include "sokol_app.h"
#	define SOKOL_IMPL

void sapp_show_keyboard(bool) {}
bool sapp_keyboard_shown() { return false; }


#elif defined(USE_SAPP)
#define SOKOL_WIN32_FORCE_MAIN
#define SOKOL_NO_ENTRY
#include "sokol_app.h"
#endif

#include "sokol_gfx.h"
#include "sokol_time.h"

#define SOKOL_IMGUI_IMPL
//#define SOKOL_IMGUI_NO_SOKOL_APP
#include "imgui.h"
#include "util/sokol_imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

