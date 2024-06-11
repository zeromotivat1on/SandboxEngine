#include "sndpch.h"
#include "EntryPoint.h"
#include "SandboxEngine/Core/Log.h"
#include "SandboxEngine/Engine/EngineLoop.h"

int snd::EntryPoint(int argc, const char** argv)
{
	Log::Init();

	EngineLoop engineLoop;
	engineLoop.Run();

	return EXIT_SUCCESS;
}
