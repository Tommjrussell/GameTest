#include "App.h"
#include <memory>

#include "sokol_gfx.h"

#include "config.h"
#ifdef USE_GLFW
#	include "glfwAppBackend.h"
#elif defined(USE_SAPP)
#	include "sappAppBackend.h"
#else
#	error "No backend API has been specified!"
#endif

tx0::App::~App()
{
}

bool tx0::App::Run(int width, int height, const char* title)
{
	AppFunctions funcs;
	funcs.initFunction = [this](){ Init(m_backend->GetSokolDesc()); return true; };
	funcs.frameFunction = [this](){ Frame(); return true; };
	funcs.shutdownFunction = [this](){ Cleanup(); return true; };

#ifdef USE_GLFW
	m_backend = std::make_unique<GlfwAppBackend>(funcs);
#elif defined(USE_SAPP)
	m_backend = std::make_unique<SappAppBackend>(funcs);
#endif

	return m_backend->Run(width, height, title);
}

int tx0::App::GetWindowWidth()
{
	return m_backend->GetWidth();
}

int tx0::App::GetWindowHeight()
{
	return m_backend->GetHeight();
}