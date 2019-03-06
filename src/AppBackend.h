#pragma once

#include <functional>

struct AppFunctions
{
	std::function<bool()> initFunction;
	std::function<bool()> frameFunction;
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

protected:
	void CallInitFunction();
	void CallFrameFunction();
	void CallShutdownFunction();

private:
	AppFunctions m_appFunctions;
};