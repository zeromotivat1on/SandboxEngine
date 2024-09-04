#include "sndpch.h"
#include "Timer.h"
#include <bx/timer.h>

static int64_t StartupCounter = snd::HighPrecisionCounter();

float snd::CurrentTime()
{
    return std::chrono::duration<float>(Timer::Clock::now().time_since_epoch()).count();
}

float snd::StartupTime()
{
    return ElapsedSeconds(StartupCounter);
}

int64_t snd::HighPrecisionCounter()
{
    return bx::getHPCounter();
}

int64_t snd::HighPrecisionFrequency()
{
    return bx::getHPFrequency();
}

float snd::ElapsedSeconds(int64_t counter)
{
    const float frequency = static_cast<float>(HighPrecisionFrequency());
    const int64_t deltaCounter = HighPrecisionCounter() - counter;
    return static_cast<float>(deltaCounter) / frequency;
}
