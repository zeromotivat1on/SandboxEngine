#include "sndpch.h"
#include "SandboxEngine/Core/Input.h"
#include <GLFW/glfw3.h>

static GLFWwindow* s_GlfwWindow = nullptr;

void snd::input::Init(Window* window)
{
	s_GlfwWindow = (GLFWwindow*)window->GetHandle();
	SND_ASSERT(s_GlfwWindow, "Given window was null or was not a GLFW window handle");
}

void snd::input::Tick(float dt)
{

}

void snd::input::Shutdown()
{
	s_GlfwWindow = nullptr;
}

bool snd::input::IsKeyPressed(KeyCode key)
{
	const int state = glfwGetKey(s_GlfwWindow, static_cast<int32_t>(key));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool snd::input::IsMouseKeyPressed(MouseCode mouse)
{
	const int state = glfwGetMouseButton(s_GlfwWindow, static_cast<int32_t>(mouse));
	return state == GLFW_PRESS;
}

float snd::input::GetMouseX()
{
	return GetMousePosition().x;
}

float snd::input::GetMouseY()
{
	return GetMousePosition().y;
}

glm::vec2 snd::input::GetMousePosition()
{
	double xpos = 0.0;
	double ypos = 0.0f;
	glfwGetCursorPos(s_GlfwWindow, &xpos, &ypos);

	return glm::vec2((float)xpos, (float)ypos);
}
