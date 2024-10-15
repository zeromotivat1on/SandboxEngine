#pragma once

// Time values use milliseconds.

#define SEC(ms)     (ms / 1000)
#define MIN(ms)     (SEC(ms) / 60)
#define HOUR(ms)    (MIN(ms) / 60)

namespace snd::time
{
    inline s64 g_HighPrecisionFrequency;

    void Init();
    void Shutdown();

    s64 Current();
    s64 SinceSystemBoot();
	s64 HighPrecisionCounter();

    struct Timer
    {
        s64     StartTime;

        void    Start();
        s64     Stop();
    };

    SND_INLINE void Timer::Start()
    {
        StartTime = Current();
    }

    SND_INLINE s64 Timer::Stop()
    {
        SND_ASSERT(StartTime != 0);
        const s64 elapsed = Current() - StartTime;
        StartTime = 0;
        return elapsed;
    }
}
