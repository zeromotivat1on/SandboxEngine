#pragma once

#ifdef BUILD_DEBUG
    #ifdef WIN32
        #define DEBUG_BREAK() __debugbreak()
    #else
        #error "Only Windows supported for now"
    #endif
#else
	#define DEBUG_BREAK()
#endif

#ifdef BUILD_DEBUG
	#define ASSERT(check) { if (!(check)) { assert_failed(MACRO_STRING(check), __FILE__, __LINE__); } }
#else
    #define ASSERT(check)
#endif

inline void assert_failed(const char* cond, const char* filename, u32 line)
{
    msg_critical("Assertion (%s) failed at (%s:%d)", cond, filename, line);
    DEBUG_BREAK();
}
