#pragma once

#ifdef SND_BUILD_DEBUG
	#define SND_DEBUG_PROFILE
#else
#endif

#ifdef SND_DEBUG_PROFILE
	#define SND_SCOPE_TIMER(Name)  snd::ScopeTimer GLUE(snd_scope_timer_, __LINE__)(Name)
#else
	#define SND_SCOPE_TIMER(Name)
#endif

#ifdef SND_DEBUG_PROFILE
namespace snd
{
	struct ScopeTimer
	{
					   ScopeTimer(const char* name);
					   ~ScopeTimer();

		const char*   Name;
		time::Timer   Timer;
	};

	SND_INLINE ScopeTimer::ScopeTimer(const char* name)
	   : Name(name)
	{
		Timer.Start();
	}

	SND_INLINE ScopeTimer::~ScopeTimer()
	{
		SND_CORE_LOG(Log, "Timer '%s' took %dms", Name, Timer.Stop());
	}
}
#endif
