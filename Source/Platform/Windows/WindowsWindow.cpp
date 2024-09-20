#include "sndpch.h"
#include "WindowsWindow.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Events/WindowEvent.h"
#include "SandboxEngine/Events/KeyEvent.h"
#include "SandboxEngine/Events/MouseEvent.h"

u8 g_GlfwWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
	SND_LOG_ERROR("GLFW Error {}: {}", error, description);
}

snd::Window* snd::Window::Create(const Props& props)
{
	return new WindowsWindow(props);
}

void snd::WindowsWindow::Init(const Window::Props& props)
{
	if (g_GlfwWindowCount == 0)
	{
		const i32 success = glfwInit();
		SND_ASSERT(success, "Failed to initialize GLFW");
		glfwSetErrorCallback(GLFWErrorCallback);
	}

	SND_LOG_INFO("Creating window \"{}\" {}x{}", props.Title, props.Width, props.Height);

	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);

	SetVsync(true);

	g_GlfwWindowCount++;

	glfwSetWindowCloseCallback	(m_Window, WindowsWindow::OnClose);
	glfwSetWindowSizeCallback	(m_Window, WindowsWindow::OnResize);
	glfwSetKeyCallback			(m_Window, WindowsWindow::OnKey);
	glfwSetCharCallback			(m_Window, WindowsWindow::OnSetChar);
	glfwSetCursorPosCallback	(m_Window, WindowsWindow::OnMouseMove);
	glfwSetScrollCallback		(m_Window, WindowsWindow::OnMouseScroll);
	glfwSetMouseButtonCallback	(m_Window, WindowsWindow::OnMouseKey);
}

void snd::WindowsWindow::Shutdown()
{
	glfwDestroyWindow(m_Window);
	g_GlfwWindowCount--;

	if (g_GlfwWindowCount == 0)
	{
		glfwTerminate();
	}
}

void snd::WindowsWindow::OnClose(GLFWwindow* window)
{
	Data& data = *static_cast<Data*>(glfwGetWindowUserPointer(window));

	WindowClosedEvent event;
	data.Callback(event);
}

void snd::WindowsWindow::OnResize(GLFWwindow* window, int width, int height)
{
	Data& data = *static_cast<Data*>(glfwGetWindowUserPointer(window));
	data.Width = width;
	data.Height = height;

	WindowResizedEvent event(width, height);
	data.Callback(event);
}

void snd::WindowsWindow::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Data& data = *static_cast<Data*>(glfwGetWindowUserPointer(window));
	const KeyCode keyCode = static_cast<KeyCode>(key);

	switch (action)
	{
	case GLFW_PRESS:
	{
		KeyPressedEvent event(keyCode, false);
		data.Callback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		KeyReleasedEvent event(keyCode);
		data.Callback(event);
		break;
	}
	case GLFW_REPEAT:
	{
		KeyPressedEvent event(keyCode, true);
		data.Callback(event);
		break;
	}
	default: SND_LOG_ERROR("Unknown key action \'{}\'", action);
	}
}

void snd::WindowsWindow::OnSetChar(GLFWwindow* window, unsigned int key)
{
	Data& data = *static_cast<Data*>(glfwGetWindowUserPointer(window));
	const KeyCode keyCode = static_cast<KeyCode>(key);

	KeyTypedEvent event(keyCode);
	data.Callback(event);
}

void snd::WindowsWindow::OnMouseMove(GLFWwindow* window, double xPos, double yPos)
{
	Data& data = *static_cast<Data*>(glfwGetWindowUserPointer(window));

	MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
	data.Callback(event);
}

void snd::WindowsWindow::OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	Data& data = *static_cast<Data*>(glfwGetWindowUserPointer(window));

	MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
	data.Callback(event);
}

void snd::WindowsWindow::OnMouseKey(GLFWwindow* window, int key, int action, int mods)
{
	Data& data = *static_cast<Data*>(glfwGetWindowUserPointer(window));
	const MouseCode mouseCode = static_cast<MouseCode>(key);

	switch (action)
	{
	case GLFW_PRESS:
	{
		MouseKeyPressedEvent event(mouseCode);
		data.Callback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		MouseKeyReleasedEvent event(mouseCode);
		data.Callback(event);
		break;
	}
	default: SND_LOG_ERROR("Unknown mouse key action \'{}\'", action);
	}
}
