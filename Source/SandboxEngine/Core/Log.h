#pragma once

#include "SandboxEngine/Core/CoreMacros.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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
		static void									Init();

		static std::shared_ptr<spdlog::logger>&		CoreLogger();

	private:
		static std::shared_ptr<spdlog::logger>		s_CoreLogger;
	};

	SND_INLINE void Log::Init()
	{
		s_CoreLogger = spdlog::stdout_color_mt("SndCore");
		s_CoreLogger->set_pattern("%^[%T] [%s:%#] [%n]: %v%$");
		s_CoreLogger->set_level(spdlog::level::trace);
	}
	
	SND_INLINE std::shared_ptr<spdlog::logger>& Log::CoreLogger()
	{
		return s_CoreLogger;
	}
}
