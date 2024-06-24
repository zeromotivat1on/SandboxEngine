#pragma once

#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Render/Camera.h"
#include <glm/glm.hpp>

namespace snd::render
{
	void Init(Window* window);
	void Shutdown();

	void Tick(float dt);

	void OnWindowResized(uint32_t width, uint32_t height);

	void SetViewTransform(int32_t viewId, const glm::mat4& view, const glm::mat4& proj);

	Camera& GetCamera();
}
