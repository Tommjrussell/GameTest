#pragma once

#include "config.h"
#include "Engine.h"

#if defined(USE_SOKOL)

#	include "sokol_gfx.h"
#	include "sokol_app.h"
#	include "sokol_time.h"

#elif defined(USE_GLFW)

#	include "glad/glad.h"
#	include "glfw/glfw3.h"
#	include "sokol_gfx.h"

#endif

namespace sal
{
	///////////////////////////////////////////////////////////////////////////////////////////////
#if defined(USE_SOKOL)

	class SokolAppDriver : public AppDriver
	{
		using AppDriver::AppDriver;
	private:
		virtual bool Run(const WindowDetails& details) override;

	private:
		uint64_t m_lastTime = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(USE_GLFW)

	class GlfwAppDriver : public AppDriver
	{
		using AppDriver::AppDriver;
	private:
		virtual bool Run(const WindowDetails& details) override;
	};

#endif

	///////////////////////////////////////////////////////////////////////////////////////////////
#if defined(USE_SOKOL) || defined(USE_GLFW)
	class SokolBaseViewIO : public ViewIO
	{
	public:
		void Setup() override;
		void OnRenderBegin() override;
		void OnRenderEnd() override;
		void Teardown() override;
	};

	struct NewBuffer
	{
		NewBuffer() { std::memset(&desc, 0, sizeof desc); }
		NewBuffer& Type(sg_buffer_type type) { desc.type = type; return *this; }
		NewBuffer& Usage(sg_usage usage) { desc.usage = usage; return *this; }
		NewBuffer& Size(int size) { desc.size = size; return *this; }
		NewBuffer& Content(const void* content) { desc.content = content; return *this; }
		sg_buffer Make() { return sg_make_buffer(&desc); }
		sg_buffer_desc desc;
	};

#endif

}