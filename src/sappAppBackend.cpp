#include "sappAppBackend.h"

#define  USE_SAPP
#ifdef USE_SAPP

#include "sokol_app.h"
#include "sokol_gfx.h"

bool SappAppBackend::Run(int width, int height, const char* title)
{
	sapp_desc sappArgs = { 0 };
	sappArgs.user_data = (void*)this;

	sappArgs.init_userdata_cb = SappAppBackend::InitCallback;
	sappArgs.frame_userdata_cb = SappAppBackend::FrameCallback;
	sappArgs.cleanup_userdata_cb = SappAppBackend::CleanupCallback;
	sappArgs.event_userdata_cb = SappAppBackend::EventCallback;
	sappArgs.fail_userdata_cb = SappAppBackend::FailCallback;

	sappArgs.width = width;
	sappArgs.height = height;
	sappArgs.sample_count = 4;
	sappArgs.gl_force_gles2 = true;
	sappArgs.window_title = title;

	int exit_code = sapp_run(&sappArgs);
	return true;
}

sg_desc SappAppBackend::GetSokolDesc() const
{
	sg_desc desc = { 0 };
	desc.mtl_device = sapp_metal_get_device();
	desc.mtl_renderpass_descriptor_cb = sapp_metal_get_renderpass_descriptor;
	desc.mtl_drawable_cb = sapp_metal_get_drawable;
	desc.d3d11_device = sapp_d3d11_get_device();
	desc.d3d11_device_context = sapp_d3d11_get_device_context();
	desc.d3d11_render_target_view_cb = sapp_d3d11_get_render_target_view;
	desc.d3d11_depth_stencil_view_cb = sapp_d3d11_get_depth_stencil_view;
	return desc;
}

void SappAppBackend::InitCallback(void* userdata)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);
	thisPtr->CallInitFunction();
}

void SappAppBackend::FrameCallback(void* userdata)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);

	thisPtr->m_windowWidth = sapp_width();
	thisPtr->m_windowHeight = sapp_height();

	thisPtr->CallFrameFunction();
}

void SappAppBackend::CleanupCallback(void* userdata)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);
	thisPtr->CallShutdownFunction();
}

void SappAppBackend::EventCallback(const sapp_event*, void* userdata)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);
}

void SappAppBackend::FailCallback(const char*, void* userdata)
{
	const auto thisPtr = reinterpret_cast<SappAppBackend*>(userdata);
}

#endif