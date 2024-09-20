#pragma once

#include "SandboxEngine/Engine/Engine.h"

namespace snd
{
	class EngineLoop final
	{
	public:
						EngineLoop();

		void			Run();

	private:
		Engine			m_Engine;
	};

	SND_INLINE f32 GetTargetFramerate()
	{
		return 1.0f / 60.0f;
	}
}
