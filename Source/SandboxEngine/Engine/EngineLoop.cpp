#include "sndpch.h"
#include "EngineLoop.h"
#include "SandboxEngine/Core/Timer.h"

snd::EngineLoop::EngineLoop()
	: m_Engine(std::unique_ptr<Window>(Window::Create({ "Sandbox Engine", 1280, 720 })))
{
}

snd::EngineLoop::~EngineLoop()
{
}

void snd::EngineLoop::Run()
{
	float dt = GetTargetFramerate();
	int64_t beginCounter = GetHighPrecisionCounter();

	while (m_Engine.IsRunning())
	{
		m_Engine.Tick(dt);

		const int64_t endCounter = GetHighPrecisionCounter();
		dt = float(endCounter - beginCounter) / float(GetHighPrecisionFrequency());

#ifdef SND_DEBUG
		// If dt is too large, we must have resumed from a breakpoint, frame-lock to the target rate this frame.
		if (dt > 1.0f)
		{
			dt = GetTargetFramerate();
		}
#endif

		beginCounter = endCounter;
	}
}
