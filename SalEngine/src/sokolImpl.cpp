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

#if defined(USE_SAPP)

#define SOKOL_NO_ENTRY

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_time.h"

#elif defined(USE_GLFW)

#include "glad/glad.h"

#if defined(USE_INTELHD3000)
// Intel HD 3000 only completely supports OpenGL 3.1 whereas, sokol requires minimum 3.3.
// The missing functionality is however available in extensions so we will use those instead.
#	undef glVertexAttribDivisor
#	define glVertexAttribDivisor glVertexAttribDivisorARB
#endif

#include "sokol_gfx.h"

#endif

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_rect_pack.h"
#include "stb_truetype.h"
