#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

#define SND_TRACE(...)		SPDLOG_LOGGER_TRACE(::snd::Log::GetCoreLogger(), __VA_ARGS__)
#define SND_INFO(...)		SPDLOG_LOGGER_INFO(::snd::Log::GetCoreLogger(), __VA_ARGS__)
#define SND_WARNING(...)	SPDLOG_LOGGER_WARN(::snd::Log::GetCoreLogger(), __VA_ARGS__)
#define SND_ERROR(...)		SPDLOG_LOGGER_ERROR(::snd::Log::GetCoreLogger(), __VA_ARGS__)
#define SND_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(::snd::Log::GetCoreLogger(), __VA_ARGS__)

namespace snd
{
	class SND_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}
