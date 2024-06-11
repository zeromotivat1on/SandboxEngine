#pragma once

#include "SandboxEngine/Core/Log.h"
#include "SandboxEngine/Core/Timer.h"

#ifdef SND_DEBUG
	#define SND_DEBUG_PROFILE
#else
#endif

#ifdef SND_DEBUG_PROFILE
	#define SND_SCOPE_TIMER(Name) ::snd::ScopeTimer GLUE(__scope_timer_, __LINE__)(Name)
#else
	#define SND_SCOPE_TIMER(Name)
#endif

namespace snd
{
	struct ScopeTimer
	{
	public:
		ScopeTimer(const char* name)
			: _Name(name)
		{
			_Timer.Start();
		}

		~ScopeTimer()
		{
			SND_LOG_DEBUG("[{}]: {:.2f}ms", _Name, _Timer.Stop<Timer::Milliseconds>());
		}

	private:
		Timer _Timer;
		const char* _Name;
	};
}
