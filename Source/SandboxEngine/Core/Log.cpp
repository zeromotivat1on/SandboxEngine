#include "sndpch.h"
#include "SandboxEngine/Core/Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

void snd::log::Init()
{
    g_CoreLogger = spdlog::stdout_color_mt("Core");
    g_CoreLogger->set_pattern("%^[%d-%m-%Y] [%T] [%L::%n] [%s:%#]: %v%$");
    g_CoreLogger->set_level(spdlog::level::trace);
}
