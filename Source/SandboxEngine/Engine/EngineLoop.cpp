#include "sndpch.h"
#include "EngineLoop.h"

snd::EngineLoop::EngineLoop()
	: m_Engine(std::unique_ptr<Window>(Window::Create({ "Sandbox Engine", 1280, 720 })))
{
}

snd::EngineLoop::~EngineLoop()
{
}

void snd::EngineLoop::Run()
{
	m_TickTimer.Start();

	while (m_Engine.IsRunning())
	{
		const float dt = m_TickTimer.Tick();

		m_Engine.Tick(dt);
	}
}
