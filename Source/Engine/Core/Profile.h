#pragma once

#ifdef BUILD_DEBUG
	#define SCOPE_TIMER(Name)	ScopeTimer MACRO_GLUE(scope_timer_, __LINE__)(Name)
#else
	#define SCOPE_TIMER(Name)
#endif

#ifdef BUILD_DEBUG
struct ScopeTimer
{
				ScopeTimer(const char* name);
				~ScopeTimer();

	const char*	name;
	u64         start;
};

inline ScopeTimer::ScopeTimer(const char* name)
	: name(name)
{
	start = time_curr();
}

inline ScopeTimer::~ScopeTimer()
{
	msg_debug("Timer (%s) took %dms", name, time_curr() - start);
}
#endif
