#include "sndpch.h"
#include "EntryPoint.h"
#include "Window.h"
#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"
#include "bgfx/bgfx.h"
#include "Log.h"

int snd::EntryPoint(int argc, const char** argv)
{
	Log::Init();

	std::unique_ptr<Window> window = std::unique_ptr<Window>(Window::Create());

	bgfx::Init bgfxInit;
	bgfxInit.type = bgfx::RendererType::Count;
	bgfxInit.platformData.nwh = window->GetNativeHandle();
	bgfxInit.resolution.width = window->GetWidth();
	bgfxInit.resolution.height = window->GetHeight();
	bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
	bgfx::init(bgfxInit);

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, uint16_t(window->GetWidth()), uint16_t(window->GetHeight()));

	while (!window->ShouldClose())
	{
		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::touch(0);

		bgfx::frame();

		window->OnUpdate();
	}

	bgfx::shutdown();

	return EXIT_SUCCESS;
}
