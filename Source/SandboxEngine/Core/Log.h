#pragma once

#include <spdlog/spdlog.h>

#define SND_CORE_TRACE(...)		SPDLOG_LOGGER_TRACE(snd::log::g_CoreLogger, __VA_ARGS__)
#define SND_CORE_DEBUG(...)		SPDLOG_LOGGER_DEBUG(snd::log::g_CoreLogger, __VA_ARGS__)
#define SND_CORE_INFO(...)		SPDLOG_LOGGER_INFO(snd::log::g_CoreLogger, __VA_ARGS__)
#define SND_CORE_WARNING(...)	SPDLOG_LOGGER_WARN(snd::log::g_CoreLogger, __VA_ARGS__)
#define SND_CORE_ERROR(...)		SPDLOG_LOGGER_ERROR(snd::log::g_CoreLogger, __VA_ARGS__)
#define SND_CORE_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(snd::log::g_CoreLogger, __VA_ARGS__)

namespace snd::log
{
    inline std::shared_ptr<spdlog::logger> g_CoreLogger;

    void Init();
}
