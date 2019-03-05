#include <memory>

class AppBackend
{

public:
	virtual ~AppBackend() {}

	virtual void Init() = 0;
	virtual void Shutdown() = 0;
};

class App
{
public:
private:
	std::unique_ptr<AppBackend> m_backend;
}