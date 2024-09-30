#include "sndpch.h"
#include "SandboxEngine/Core/Timer.h"
#include <bx/timer.h>

i64 g_StartupCounter = snd::HighPrecisionCounter();

f32 snd::CurrentTime()
{
    return std::chrono::duration<f32>(Timer::Clock::now().time_since_epoch()).count();
}

f32 snd::StartupTime()
{
    return ElapsedSeconds(g_StartupCounter);
}

i64 snd::HighPrecisionCounter()
{
    return bx::getHPCounter();
}

i64 snd::HighPrecisionFrequency()
{
    return bx::getHPFrequency();
}

f32 snd::ElapsedSeconds(i64 counter)
{
    const f32 frequency = static_cast<f32>(HighPrecisionFrequency());
    const i64 deltaCounter = HighPrecisionCounter() - counter;
    return static_cast<f32>(deltaCounter) / frequency;
}
