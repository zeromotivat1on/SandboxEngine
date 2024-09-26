#pragma once

#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Ecs/Entity.h"

namespace snd
{
	class	Camera;
	struct 	CameraComponent;
	struct 	TransformComponent;
}

namespace snd::render
{
	void Init(Window* window);
	void Shutdown();

	void Tick(f32 dt);

	void OnWindowResized(u16 width, u16 height);

	void SetCamera(const CameraComponent* camera);

	// Create debug cube entity with default transform.
	Entity NewEntityDebugCube();
}
