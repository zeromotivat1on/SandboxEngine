#pragma once

#ifdef SND_DEBUG
	#define SND_ENABLE_ASSERT
	#ifdef SND_PLATFORM_WINDOWS
		#define SND_DEBUG_BREAK()								__debugbreak()
	#else
		#error													"Only Windows supported"
	#endif
#else
	#define SND_DEBUG_BREAK()
#endif

#ifdef SND_ENABLE_ASSERT
	#define SND_ASSERT_EXPAND_MACRO(x)							x

	#define SND_ASSERT_IMPL(check, msg, ...)					{ if(!(check)) { SND_LOG_ERROR(msg, __VA_ARGS__); SND_DEBUG_BREAK(); } }

	#define SND_ASSERT_WITH_MSG(check, ...)						SND_ASSERT_IMPL(check, "Assertion failed: {0}", __VA_ARGS__)
	#define SND_ASSERT_NO_MSG(check)							SND_ASSERT_IMPL(check, "Assertion '{0}' failed at {1}:{2}", SND_STRINGIFY_MACRO(check), __FILE__, __LINE__)

	#define SND_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...)	macro
	#define SND_ASSERT_GET_MACRO(...)							SND_EXPAND_MACRO(SND_ASSERT_GET_MACRO_NAME(__VA_ARGS__, SND_ASSERT_WITH_MSG, SND_ASSERT_NO_MSG))

	#define SND_ASSERT(...)										SND_ASSERT_EXPAND_MACRO(SND_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__))
#else
	#define SND_ASSERT(...)
#endif
