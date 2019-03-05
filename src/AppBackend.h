#pragma once

#include <functional>

class AppBackend
{

public:
	virtual ~AppBackend() {}

	virtual void Init(const char* title, int width, int height) = 0;
	virtual void Loop(std::function<void()>& frameFunc) = 0;
	virtual void Shutdown() = 0;
};