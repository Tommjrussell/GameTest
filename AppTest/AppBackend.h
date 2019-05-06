#pragma once

#include <functional>

#include "sokol_app.h" // for the event struct

struct sg_desc;

struct AppFunctions
{
	std::function<bool()> initFunction;
	std::function<bool()> frameFunction;
	std::function<bool(const sapp_event*)> eventFunction;
	std::function<bool()> shutdownFunction;
};

class AppBackend
{
public:
	AppBackend(AppFunctions appFunctions)
		: m_appFunctions(appFunctions)
	{
	}

	virtual ~AppBackend() {}

	virtual bool Run(int width, int height, const char* title) = 0;

	int GetWidth() const { return m_windowWidth; }
	int GetHeight() const { return m_windowHeight; }

	virtual sg_desc GetSokolDesc() const = 0;

protected:
	void CallInitFunction();
	void CallFrameFunction();
	void CallEventFunction(const sapp_event* event);
	void CallShutdownFunction();

private:
	AppFunctions m_appFunctions;

protected:
	int m_windowWidth = 0;
	int m_windowHeight = 0;

};