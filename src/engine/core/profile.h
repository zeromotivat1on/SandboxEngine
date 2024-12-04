#pragma once

#ifdef BUILD_RELEASE
#define SCOPE_TIMER(Name)	
#else
#define SCOPE_TIMER(Name)	Scope_Timer MACRO_GLUE(scope_timer_, __LINE__)(Name)

struct Scope_Timer
{
				Scope_Timer(const char* name);
				~Scope_Timer();

	const char*	name;
	u64         start;
};

inline Scope_Timer::Scope_Timer(const char* name)
	: name(name)
{
	start = current_time();
}

inline Scope_Timer::~Scope_Timer()
{
	msg_debug("Timer (%s) took %ums", name, current_time() - start);
}
#endif
