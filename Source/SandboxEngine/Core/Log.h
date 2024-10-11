#pragma once

#include <spdlog/spdlog.h>

#define SND_CORE_TRACE(...)		SPDLOG_LOGGER_TRACE(snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_CORE_DEBUG(...)		SPDLOG_LOGGER_DEBUG(snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_CORE_INFO(...)		SPDLOG_LOGGER_INFO(snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_CORE_WARNING(...)	SPDLOG_LOGGER_WARN(snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_CORE_ERROR(...)		SPDLOG_LOGGER_ERROR(snd::Log::CoreLogger(), __VA_ARGS__)
#define SND_CORE_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(snd::Log::CoreLogger(), __VA_ARGS__)

namespace snd
{
	class Log
	{
	public:
		static void									Init();
		static std::shared_ptr<spdlog::logger>&		CoreLogger();

	private:
		static std::shared_ptr<spdlog::logger>		s_CoreLogger;
	};

	SND_INLINE std::shared_ptr<spdlog::logger>& Log::CoreLogger()
	{
		return s_CoreLogger;
	}
}
