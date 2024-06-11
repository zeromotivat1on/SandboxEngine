#pragma once

#include "SandboxEngine/Engine/Engine.h"
#include "SandboxEngine/Core/Timer.h"

namespace snd
{
	class EngineLoop final
	{
	public:
		EngineLoop();
		~EngineLoop();

		void Run();

	private:
		Engine m_Engine;
		Timer m_TickTimer;
	};
}
