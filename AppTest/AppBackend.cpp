#include "AppBackend.h"

void AppBackend::CallInitFunction()
{
	if (bool(m_appFunctions.initFunction))
	{
		m_appFunctions.initFunction();
	}
}

void AppBackend::CallFrameFunction()
{
	if (bool(m_appFunctions.frameFunction))
	{
		m_appFunctions.frameFunction();
	}
}

void AppBackend::CallEventFunction(const sapp_event* event)
{
	if (bool(m_appFunctions.eventFunction))
	{
		m_appFunctions.eventFunction(event);
	}
}

void AppBackend::CallShutdownFunction()
{
	if (bool(m_appFunctions.shutdownFunction))
	{
		m_appFunctions.shutdownFunction();
	}
}