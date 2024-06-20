#include "sndpch.h"
#include "SandboxEngine/Core/Timer.h"
#include <bx/timer.h>

int64_t snd::GetHighPrecisionCounter()
{
	return bx::getHPCounter();
}

int64_t snd::GetHighPrecisionFrequency()
{
	return bx::getHPFrequency();
}

float snd::GetElapsedSeconds(int64_t counter)
{
	const float frequency = static_cast<float>(GetHighPrecisionFrequency());
	const int64_t deltaCounter = GetHighPrecisionCounter() - counter;
	return static_cast<float>(deltaCounter) / frequency;
}
