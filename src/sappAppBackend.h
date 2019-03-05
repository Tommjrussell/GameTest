#pragma once

#include "AppBackend.h"

#include <string>

class SappAppBackend : public AppBackend
{
public:
	virtual void Init(const char* title, int width, int height) override;
	virtual void Loop(std::function<void()>& frameFunc) override;
	virtual void Shutdown() override;

private:
	int m_width;
	int m_height;
	std::string m_title;
};
