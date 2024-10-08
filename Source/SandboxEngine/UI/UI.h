#pragma once

#include "SandboxEngine/Core/Window.h"

namespace snd::ui
{
	void Init(Window* window);
	void Shutdown();

	void Tick(f32 dt);
}