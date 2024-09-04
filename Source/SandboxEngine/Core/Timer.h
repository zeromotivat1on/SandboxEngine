#pragma once

#include <chrono>

namespace snd
{
	// Get current time in seconds.
	float CurrentTime();

	// Get time in seconds since engine startup.
	float StartupTime();
    
	int64_t HighPrecisionCounter();
	int64_t HighPrecisionFrequency();
	float ElapsedSeconds(int64_t counter); // get passed seconds since given high precision counter
	
	// Encapsulates basic usage of chrono, providing a means to calculate float durations between time points via function calls.
	class Timer
	{
	public:
		using Clock = std::chrono::steady_clock;
		using Seconds = std::ratio<1>;
		using Milliseconds = std::ratio<1, 1000>;
		using Microseconds = std::ratio<1, 1000000>;
		using Nanoseconds = std::ratio<1, 1000000000>;
		using DefaultResolution = Seconds;

	public:
		Timer() : m_StartTime(Clock::now()), m_PreviousTick(Clock::now()) {}

		virtual ~Timer() = default;

	public:
		inline bool Running() const { return m_Running; }

		// Starts the timer, Elapsed() now returns the duration since Start().
		void Start()
		{
			if (!m_Running)
			{
				m_Running = true;
				m_StartTime = Clock::now();
			}
		}

		// Laps the timer, Elapsed() now returns the duration since the last Lap().
		void Lap()
		{
			m_Lapping = true;
			m_LapTime = Clock::now();
		}

		// Stops the timer, Elapsed() now returns 0.
		// @return The total execution time between Start() and Stop().
		template <typename T = DefaultResolution>
		float Stop()
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

		// Calculates the time difference between now and when the timer was started, if Lap() was called, then between now and when the timer was last lapped.
		// @return The duration between the two time points (default in seconds).
		template <typename T = DefaultResolution>
		float Elapsed()
		{
			if (!m_Running)
			{
				return 0;
			}

			Clock::time_point start = m_StartTime;

			if (m_Lapping)
			{
				start = m_LapTime;
			}

			return std::chrono::duration<float, T>(Clock::now() - start).count();
		}

		// Calculates the time difference between now and the last time this function was called
		// @return The duration between the two time points (default in seconds)
		template <typename T = DefaultResolution>
		float Tick()
		{
			const auto now = Clock::now();
			const auto duration = std::chrono::duration<float, T>(now - m_PreviousTick);
			m_PreviousTick = now;
			return duration.count();
		}

	private:
		bool m_Running = false;
		bool m_Lapping = false;
		Clock::time_point m_StartTime;
		Clock::time_point m_LapTime;
		Clock::time_point m_PreviousTick;
	};
}
