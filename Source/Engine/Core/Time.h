#pragma once

// Time values use milliseconds.

#define SEC(ms)     (ms / 1000)
#define MIN(ms)     (SEC(ms) / 60)
#define HOUR(ms)    (MIN(ms) / 60)

namespace snd
{
    inline u64 gHighPrecisionFrequency;

    u64 CurrentTime();
    u64 TimeSinceSystemBoot();
	u64 HighPrecisionCounter();
	u64 HighPrecisionFrequency();

    struct Timer
    {
        u64     StartTime;

        void    Start();
        u64     Stop();
    };

    SND_INLINE void Timer::Start()
    {
        StartTime = CurrentTime();
    }

    SND_INLINE u64 Timer::Stop()
    {
        SND_ASSERT(StartTime != 0);
        const u64 elapsed = CurrentTime() - StartTime;
        StartTime = 0;
        return elapsed;
    }
}
