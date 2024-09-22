#pragma once

#include "SandboxEngine/Core/Window.h"

namespace snd
{
	class Camera;
	class CameraComponent;
}

namespace snd::render
{
	void Init(Window* window);
	void Shutdown();

	void Tick(f32 dt);

	void OnWindowResized(u16 width, u16 height);

	void SetCamera(const CameraComponent* camera);
}
