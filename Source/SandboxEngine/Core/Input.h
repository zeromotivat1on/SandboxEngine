#pragma once

#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Events/KeyEvent.h"
#include "SandboxEngine/Events/MouseEvent.h"

namespace snd::input
{
	void Init(Window* window);
	void Shutdown();

	bool IsKeyPressed(KeyCode key);
	bool IsMouseKeyPressed(MouseCode button);
	float GetMouseX();
	float GetMouseY();
	glm::vec2 GetMousePosition();
}
