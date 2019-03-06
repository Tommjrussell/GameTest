#pragma once

#include "AppBackend.h"

#include <string>

struct sapp_event;

class SappAppBackend : public AppBackend
{
public:
	SappAppBackend(AppFunctions appFunctions)
		: AppBackend(appFunctions)
	{
	}

	virtual bool Run(int width, int height, const char* title) override;

private:
	static void sappCallbacks::InitCallback(void* userdata);
	static void sappCallbacks::FrameCallback(void* userdata);
	static void sappCallbacks::CleanupCallback(void* userdata);
	static void sappCallbacks::EventCallback(const sapp_event*, void*);
	static void sappCallbacks::FailCallback(const char*, void*);

private:
	int m_width;
	int m_height;
	std::string m_title;
};
