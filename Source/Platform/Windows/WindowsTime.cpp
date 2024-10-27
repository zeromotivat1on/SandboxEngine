#include "sndpch.h"
#include "Engine/Core/Time.h"

u64 snd::CurrentTime()
{
    return HighPrecisionCounter() * 1000ULL / gHighPrecisionFrequency;
}

u64 snd::TimeSinceSystemBoot()
{
    return GetTickCount64();
}

u64 snd::HighPrecisionCounter()
{
    LARGE_INTEGER counter;
    const BOOL res = QueryPerformanceCounter(&counter);
    SND_ASSERT(res);
    return counter.QuadPart;
}

u64 snd::HighPrecisionFrequency()
{
    LARGE_INTEGER frequency;
    const BOOL res = QueryPerformanceFrequency(&frequency);
    SND_ASSERT(res);
    return frequency.QuadPart;
}
