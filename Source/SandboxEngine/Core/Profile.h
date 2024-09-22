#pragma once

#include "SandboxEngine/Core/Log.h"
#include "SandboxEngine/Core/Timer.h"
#include "SandboxEngine/Core/CoreMacros.h"

#ifdef SND_BUILD_DEBUG
	#define SND_DEBUG_PROFILE
#else
#endif

#ifdef SND_DEBUG_PROFILE
	#define SND_SCOPE_TIMER(Name)		::snd::ScopeTimer GLUE(__snd_scope_timer_, __LINE__)(Name)
	#define SND_SCOPE_FUNCTION_TIMER()	::snd::ScopeTimer GLUE(__snd_scope_timer_, __LINE__)(__FUNCTION__)
#else
	#define SND_SCOPE_TIMER(Name)
	#define SND_SCOPE_FUNCTION_TIMER()
#endif

#ifdef SND_DEBUG_PROFILE
namespace snd
{
	struct ScopeTimer
	{
	public:
							ScopeTimer(const char* name);
							~ScopeTimer();

	private:
		Timer				m_Timer;
		const char*			m_Name;
	};

	SND_INLINE ScopeTimer::ScopeTimer(const char* name): m_Name(name)
	{
		m_Timer.Start();
	}

	SND_INLINE ScopeTimer::~ScopeTimer()
	{
		SND_DEBUG("[{}]: {:.2f}ms", m_Name, m_Timer.Stop<Timer::Milliseconds>());
	}
}
#endif
