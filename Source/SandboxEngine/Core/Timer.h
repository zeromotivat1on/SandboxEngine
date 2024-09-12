#pragma once

#include "SandboxEngine/Core/CoreMacros.h"
#include <chrono>

namespace snd
{
	// Get current time in seconds.
	float	CurrentTime();

	// Get time in seconds since engine startup.
	float	StartupTime();
    
	int64_t HighPrecisionCounter();
	int64_t HighPrecisionFrequency();
	float	ElapsedSeconds(int64_t counter); // get passed seconds since given high precision counter
	
	// Encapsulates basic usage of chrono, providing a means to calculate float durations between time points via function calls.
	class Timer
	{
	public:
		using Clock				= std::chrono::steady_clock;
		using Seconds			= std::ratio<1>;
		using Milliseconds		= std::ratio<1, 1000>;
		using Microseconds		= std::ratio<1, 1000000>;
		using Nanoseconds		= std::ratio<1, 1000000000>;
		using DefaultResolution	= Seconds;

	public:
								Timer();
		virtual					~Timer() = default;

		bool					Running() const;

		void					Start();
		void					Lap();

		template <typename T = DefaultResolution>
		float					Stop();

		template <typename T = DefaultResolution>
		float					Elapsed();

		template <typename T = DefaultResolution>
		float					Tick();

	private:
		bool 					m_Running = false;
		bool 					m_Lapping = false;
		Clock::time_point 		m_StartTime;
		Clock::time_point 		m_LapTime;
		Clock::time_point		m_PreviousTick;
	};

	SND_INLINE Timer::Timer()
		: m_StartTime(Clock::now()), m_PreviousTick(Clock::now())
	{}

	SND_INLINE bool Timer::Running() const
	{
		return m_Running;
	}

	SND_INLINE void Timer::Start()
	{
		if (!m_Running)
		{
			m_Running = true;
			m_StartTime = Clock::now();
		}
	}

	SND_INLINE void Timer::Lap()
	{
		m_Lapping = true;
		m_LapTime = Clock::now();
	}

	template <typename T>
	float Timer::Stop()
	{
		if (!m_Running)
		{
			return 0;
		}

		m_Running = false;
		m_Lapping = false;

		const auto duration = std::chrono::duration<float, T>(Clock::now() - m_StartTime);
		m_StartTime = Clock::now();
		m_LapTime = Clock::now();

		return duration.count();
	}

	template <typename T>
	float Timer::Elapsed()
	{
		if (!m_Running)
		{
			return 0;
		}

		const Clock::time_point start = m_Lapping ? m_LapTime : m_StartTime;
		return std::chrono::duration<float, T>(Clock::now() - start).count();
	}

	template <typename T>
	float Timer::Tick()
	{
		const auto now = Clock::now();
		const auto duration = std::chrono::duration<float, T>(now - m_PreviousTick);
		m_PreviousTick = now;
		return duration.count();
	}
}
