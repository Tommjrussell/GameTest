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

void AppBackend::CallShutdownFunction()
{
	if (bool(m_appFunctions.shutdownFunction))
	{
		m_appFunctions.shutdownFunction();
	}
}