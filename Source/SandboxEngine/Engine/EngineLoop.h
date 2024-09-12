#pragma once

#include "SandboxEngine/Engine/Engine.h"

namespace snd
{
	inline float GetTargetFramerate() { return 1.0f / 60.0f; }

	class EngineLoop final
	{
	public:
						EngineLoop();
						~EngineLoop();

		void			Run();

	private:
		Engine			m_Engine;
	};
}
