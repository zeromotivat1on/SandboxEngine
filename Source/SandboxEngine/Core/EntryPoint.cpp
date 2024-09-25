#include "sndpch.h"
#include "SandboxEngine/Core/EntryPoint.h"
#include "SandboxEngine/Engine/EngineLoop.h"

int snd::EntryPoint(int argc, const char** argv)
{
	EngineLoop engineLoop;
	engineLoop.Init();
	engineLoop.Run();
	engineLoop.Shutdown();
	
	return EXIT_SUCCESS;
}
