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

	virtual sg_desc GetSokolDesc() const override;

private:
	static void InitCallback(void* userdata);
	static void FrameCallback(void* userdata);
	static void CleanupCallback(void* userdata);
	static void EventCallback(const sapp_event* event, void* userdata);
	static void FailCallback(const char*, void* userdata);

private:
	int m_width;
	int m_height;
	std::string m_title;
};