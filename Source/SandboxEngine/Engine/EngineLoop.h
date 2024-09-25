#pragma once

#include "SandboxEngine/Engine/Engine.h"

namespace snd
{
	class EngineLoop final
	{
	public:
		void			Init();
		void			Run();
		void			Shutdown();

	private:
		Engine			m_Engine;
	};

	SND_INLINE f32 GetTargetFramerate()
	{
		return 1.0f / 60.0f;
	}
}
