#pragma once

namespace snd
{
	f32	CurrentTime(); // get current time in seconds
	f32	StartupTime(); // get time in seconds since engine startup
    
	i64 HighPrecisionCounter();
	i64 HighPrecisionFrequency();
	f32	ElapsedSeconds(i64 counter); // get passed seconds since given high precision counter
	
	// Encapsulates basic usage of chrono, providing a means to calculate f32 durations between time points via function calls.
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
		f32						Stop();

		template <typename T = DefaultResolution>
		f32						Elapsed();

		template <typename T = DefaultResolution>
		f32						Tick();

	private:
		bool 					m_Running;
		bool 					m_Lapping;
		Clock::time_point 		m_StartTime;
		Clock::time_point 		m_LapTime;
		Clock::time_point		m_PreviousTick;
	};

	SND_INLINE Timer::Timer()
		: m_Running(false), m_Lapping(false), m_StartTime(Clock::now()), m_PreviousTick(Clock::now())
	{
	}

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
	f32 Timer::Stop()
	{
		if (!m_Running)
		{
			return 0;
		}

		m_Running = false;
		m_Lapping = false;

		const auto duration = std::chrono::duration<f32, T>(Clock::now() - m_StartTime);
		m_StartTime = Clock::now();
		m_LapTime = Clock::now();

		return duration.count();
	}

	template <typename T>
	f32 Timer::Elapsed()
	{
		if (!m_Running)
		{
			return 0;
		}

		const Clock::time_point start = m_Lapping ? m_LapTime : m_StartTime;
		return std::chrono::duration<f32, T>(Clock::now() - start).count();
	}

	template <typename T>
	f32 Timer::Tick()
	{
		const auto now = Clock::now();
		const auto duration = std::chrono::duration<f32, T>(now - m_PreviousTick);
		m_PreviousTick = now;
		return duration.count();
	}
}
