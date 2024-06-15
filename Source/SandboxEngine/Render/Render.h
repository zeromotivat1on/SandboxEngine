#pragma once

#include "SandboxEngine/Core/Window.h"

namespace snd::render
{
	void Init(Window* window);
	void Shutdown();

	void Tick(float dt);

	void OnWindowResized(uint32_t width, uint32_t height);
}
