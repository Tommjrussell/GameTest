#include "sappAppBackend.h"

#include "sokol_app.h"

void SappAppBackend::Init(const char* title, int width, int height)
{

}

void SappAppBackend::Loop(std::function<void()>& frameFunc)
{
	sapp_desc sappArgs = {0};
	args.user_data = state;
	args.init_userdata_cb = init;
	args.frame_userdata_cb = frame;
	args.cleanup_cb = cleanup;          /* cleanup doesn't need access to the state struct */
	args.width = width;
	args.height = height;
	args.sample_count = 4;
	args.gl_force_gles2 = true;
	args.window_title = title,

    int exit_code = sapp_run(args);
}

void SappAppBackend::Shutdown()
{
}
