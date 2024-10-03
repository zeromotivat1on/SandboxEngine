#include "sndpch.h"
#include "SandboxEngine/Core/Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> snd::Log::s_CoreLogger;

void snd::Log::Init()
{
    s_CoreLogger = spdlog::stdout_color_mt("SndCore");
    s_CoreLogger->set_pattern("%^[%d-%m-%Y] [%T] [%n:%L] [%s:%#]: %v%$");
    s_CoreLogger->set_level(spdlog::level::trace);
}
