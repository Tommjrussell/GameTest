#pragma once

#include "AppBackend.h"
#include <memory>


struct sg_desc;

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

	protected:
		int GetWindowWidth();
		int GetWindowHeight();

	private:
		std::unique_ptr<AppBackend> m_backend;
	};

}