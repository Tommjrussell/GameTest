#include "sappAppBackend.h"

#include "sokol_app.h"

bool SappAppBackend::Run(int width, int height, const char* title)
{
	sapp_desc sappArgs = { 0 };
	args.user_data = (void*)this;

	args.init_userdata_cb = SappAppBackend::InitCallback;
	args.frame_userdata_cb = SappAppBackend::FrameCallback;
	args.cleanup_userdata_cb = SappAppBackend::CleanupCallback;
	args.event_userdata_cb = SappAppBackend::EventCallback;
	args.fail_userdata_cb = SappAppBackend::FailCallback;

	args.width = width;
	args.height = height;
	args.sample_count = 4;
	args.gl_force_gles2 = true;
	args.window_title = title,

	int exit_code = sapp_run(args);
}

void SappAppBackend::InitCallback(void* userdata)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);
	thisPtr->CallInitFunction();
}

void SappAppBackend::FrameCallback(void* userdata)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);
	thisPtr->CallFrameFunction();
}

void SappAppBackend::CleanupCallback(void* userdata)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);
	thisPtr->CallShutdownFunction();
}

void SappAppBackend::EventCallback(const sapp_event*, void*)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);
}

void SappAppBackend::FailCallback(const char*, void*)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);
}