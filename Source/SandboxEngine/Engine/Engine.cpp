#include "sndpch.h"
#include "Engine.h"
#include <bgfx/bgfx.h>

snd::Engine::Engine(std::unique_ptr<Window>&& window)
	: m_Window(std::move(window))
{
	Init();
}

snd::Engine::~Engine()
{
	Shutdown();
}

bool snd::Engine::IsRunning() const
{
	return !m_Window->ShouldClose();
}

void snd::Engine::Init()
{
	SND_SCOPE_TIMER(__FUNCTION__);

	m_Window->SetEventCallback(SND_BIND_EVENT_FN(Engine::OnEvent));

	bgfx::Init bgfxInit;
	bgfxInit.type = bgfx::RendererType::Count;
	bgfxInit.platformData.nwh = m_Window->GetNativeHandle();
	bgfxInit.resolution.width = m_Window->GetWidth();
	bgfxInit.resolution.height = m_Window->GetHeight();

	if (m_Window->IsVsync())
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

void snd::Engine::Shutdown()
{
	bgfx::shutdown();
}

void snd::Engine::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(SND_BIND_EVENT_FN(Engine::OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(SND_BIND_EVENT_FN(Engine::OnWindowResize));
}

bool snd::Engine::OnWindowClose(WindowCloseEvent& event)
{
	SND_LOG_INFO("Closing window \"{}\"", m_Window->GetTitle());
	return true;
}

bool snd::Engine::OnWindowResize(WindowResizeEvent& event)
{
	uint32_t flags = 0;

	if (m_Window->IsVsync())
	{
		flags |= BGFX_RESET_VSYNC;
	}

	bgfx::reset(event.GetWidth(), event.GetHeight(), flags);

	return true;
}

void snd::Engine::Tick(float dt)
{
	bgfx::setViewRect(0, 0, 0, uint16_t(m_Window->GetWidth()), uint16_t(m_Window->GetHeight()));

	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	bgfx::dbgTextClear();
	bgfx::dbgTextPrintf(1, 1, 0x0f, "Window size: %dx%d", m_Window->GetWidth(), m_Window->GetHeight());
	bgfx::dbgTextPrintf(1, 2, 0x0f, "Delta time: %.2fms", (dt * 1000.0f));
	bgfx::dbgTextPrintf(1, 3, 0x0f, "FPS: %.2f", (1.0f / dt));
	bgfx::dbgTextPrintf(1, 4, 0x0f, "Vsync: %s", m_Window->IsVsync() ? "ON" : "OFF");
	
	bgfx::frame();

	m_Window->OnUpdate();
}
