#pragma once

#ifdef BUILD_RELEASE
#define SCOPE_TIMER(Name)	
#else
#define SCOPE_TIMER(Name)	ScopeTimer MACRO_GLUE(scope_timer_, __LINE__)(Name)

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
	msg_debug("Timer (%s) took %ums", name, time_curr() - start);
}
#endif
