#include "sndpch.h"
#include "SandboxEngine/Engine/EngineLoop.h"
#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Core/Timer.h"

void snd::EngineLoop::Init()
{
	log::Init();

	Window* window = Window::Create({ "Sandbox Engine", 1280, 720 });
	m_Engine.Init(window);
}

void snd::EngineLoop::Run()
{
	f32 dt = GetTargetFramerate();
	s64 beginCounter = HighPrecisionCounter();

	while (m_Engine.Running())
	{
		m_Engine.Tick(dt);

		const s64 endCounter = HighPrecisionCounter();
		dt = static_cast<float>(endCounter - beginCounter) / static_cast<float>(HighPrecisionFrequency());

#ifdef SND_BUILD_DEBUG
		// If dt is too large, we must have resumed from a breakpoint, frame-lock to the target rate this frame.
		if (dt > 1.0f)
		{
			dt = GetTargetFramerate();
		}
#endif

		beginCounter = endCounter;
	}
}

void snd::EngineLoop::Shutdown()
{
	m_Engine.Shutdown();
}
