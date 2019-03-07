#pragma once

#include "AppBackend.h"

struct GLFWwindow;

class GlfwAppBackend : public AppBackend
{
public:
	GlfwAppBackend(AppFunctions appFunctions)
		: AppBackend(appFunctions)
	{
	}

	virtual sg_desc GetSokolDesc() const override;

	virtual bool Run(int width, int height, const char* title) override;
};
