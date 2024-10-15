#include "sndpch.h"
#include "SandboxEngine/Core/Time.h"

namespace snd::time
{
    void Init()
    {
        LARGE_INTEGER frequency;
        const BOOL res = QueryPerformanceFrequency(&frequency);
        SND_ASSERT(res);
        g_HighPrecisionFrequency = frequency.QuadPart;
    }

    void Shutdown()
    {
        g_HighPrecisionFrequency = 0;
    }

    s64 Current()
    {
        return HighPrecisionCounter() * 1000LL / g_HighPrecisionFrequency;
    }

    s64 SinceSystemBoot()
    {
        return GetTickCount64();
    }

    s64 HighPrecisionCounter()
    {
        LARGE_INTEGER counter;
        const BOOL res = QueryPerformanceCounter(&counter);
        SND_ASSERT(res);
        return counter.QuadPart;
    }
}