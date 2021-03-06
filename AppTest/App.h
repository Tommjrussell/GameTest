#pragma once

#include "AppBackend.h"
#include <memory>

#include "sokol_gfx.h"
#include "config.h"

namespace tx0
{
	class App
	{
	public:
		~App();
		bool Run(int width, int height, const char* title);

	private:
		virtual void Init(sg_desc args) = 0;
		virtual void Cleanup() = 0;
		virtual void Frame() = 0;
		virtual void Event(const sapp_event* event) = 0;

	protected:
		int GetWindowWidth();
		int GetWindowHeight();

	private:
		std::unique_ptr<AppBackend> m_backend;
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

}