#pragma once

#include "Appbackend.h"
#include <memory>

struct sg_desc;

namespace tx0
{
	class App
	{
		bool Run();

	private:
		virtual void Init(sg_desc args) = 0;
		virtual void Cleanup() = 0;
		virtual void Frame() = 0;

	protected:
		int GetWindowWidth();
		int GetWindowHeight();

	private:
		std::unique_ptr<AppBackend> m_backend;
	};

}