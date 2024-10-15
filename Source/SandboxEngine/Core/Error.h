#pragma once

#ifdef SND_BUILD_DEBUG
	#define SND_ENABLE_ASSERT
	#ifdef SND_PLATFORM_WINDOWS
		#define SND_DEBUG_BREAK()								__debugbreak()
	#else
		#error													"Only Windows supported for now"
	#endif
#else
	#define SND_DEBUG_BREAK()
#endif

#ifdef SND_ENABLE_ASSERT
	#define SND_ASSERT(check)  { if (!(check)) { snd::AssertFailed(SND_STRINGIFY_MACRO(check), __FILE__, __LINE__); } }
#else
	#define SND_ASSERT(check)
#endif

namespace snd
{
    SND_INLINE void AssertFailed(const char* cond, const char* filename, u32 line)
    {
        SND_CORE_LOG(Error, "Assertion '%s' failed at '%s:%d'", cond, filename, line);
        SND_DEBUG_BREAK();
    }
}
