#include "sndpch.h"
#include "SandboxEngine/Render/Render.h"
#include <bgfx/bgfx.h>

// Current window we render on.
static snd::Window* s_Window = nullptr;

void snd::render::Init(Window* window)
{
	s_Window = window;

	bgfx::Init bgfxInit;
	bgfxInit.type = bgfx::RendererType::Count;
	bgfxInit.platformData.nwh = window->GetNativeHandle();
	bgfxInit.resolution.width = window->GetWidth();
	bgfxInit.resolution.height = window->GetHeight();

	if (window->IsVsync())
	{
		bgfxInit.resolution.reset |= BGFX_RESET_VSYNC;
	}

	bgfx::init(bgfxInit);

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);

#ifdef SND_DEBUG
	bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

	SND_LOG_INFO("Using Renderer API \"{}\"", bgfx::getRendererName(bgfx::getRendererType()));
}

void snd::render::Shutdown()
{
	bgfx::shutdown();
}

void snd::render::OnWindowResized(uint32_t width, uint32_t height)
{
	uint32_t flags = 0;

	if (s_Window->IsVsync())
	{
		flags |= BGFX_RESET_VSYNC;
	}

	bgfx::reset(width, height, flags);
}

void snd::render::Tick(float dt)
{
	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	// Clear buffer for debug info.
	bgfx::dbgTextClear();

	bgfx::setViewRect(0, 0, 0, uint16_t(s_Window->GetWidth()), uint16_t(s_Window->GetHeight()));

	bgfx::dbgTextPrintf(1, 1, 0x0f, "Window size: %dx%d", s_Window->GetWidth(), s_Window->GetHeight());
	bgfx::dbgTextPrintf(1, 2, 0x0f, "Delta time: %.2fms", (dt * 1000.0f));
	bgfx::dbgTextPrintf(1, 3, 0x0f, "FPS: %.2f", (1.0f / dt));
	bgfx::dbgTextPrintf(1, 4, 0x0f, "Vsync: %s", s_Window->IsVsync() ? "ON" : "OFF");

	bgfx::frame();
}
