#pragma once

#include <spdlog/spdlog.h>

#define SND_LOG_TRACE(...)		SPDLOG_LOGGER_TRACE(::snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_LOG_DEBUG(...)		SPDLOG_LOGGER_DEBUG(::snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_LOG_INFO(...)		SPDLOG_LOGGER_INFO(::snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_LOG_WARNING(...)	SPDLOG_LOGGER_WARN(::snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_LOG_ERROR(...)		SPDLOG_LOGGER_ERROR(::snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_LOG_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(::snd::Log::CoreLogger(), __VA_ARGS__)

namespace snd
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& CoreLogger() { return s_CoreLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}
