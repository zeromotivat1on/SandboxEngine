#include "sndpch.h"
#include "WindowsWindow.h"
#include "SandboxEngine/Events/WindowEvent.h"
#include "SandboxEngine/Events/KeyEvent.h"
#include "SandboxEngine/Events/MouseEvent.h"

u8 g_GlfwWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
	SND_ERROR("GLFW Error {}: {}", error, description);
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

	SND_INFO("Creating window \"{}\" {}x{}", props.Title, props.Width, props.Height);

	m_Title = props.Title;
	m_Width = props.Width;
	m_Height = props.Height;

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	SetVsync(true);

	g_GlfwWindowCount++;

	glfwSetWindowCloseCallback	(m_Window, WindowsWindow::OnClose);
	glfwSetWindowSizeCallback	(m_Window, WindowsWindow::OnResize);
	glfwSetKeyCallback			(m_Window, WindowsWindow::OnKey);
	glfwSetCharCallback			(m_Window, WindowsWindow::OnSetChar);
	glfwSetCursorPosCallback	(m_Window, WindowsWindow::OnMouseMove);
	glfwSetScrollCallback		(m_Window, WindowsWindow::OnMouseScroll);
	glfwSetMouseButtonCallback	(m_Window, WindowsWindow::OnMouseKey);
	glfwSetWindowFocusCallback	(m_Window, WindowsWindow::OnFocus);
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
	WindowsWindow& win = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
	WindowClosedEvent event;
	win.m_Callback(event);
}

void snd::WindowsWindow::OnResize(GLFWwindow* window, int width, int height)
{
	WindowsWindow& win = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
	win.m_Width = width;
	win.m_Height = height;

	WindowResizedEvent event(width, height);
	win.m_Callback(event);
}

void snd::WindowsWindow::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowsWindow& win = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
	const KeyCode keyCode = static_cast<KeyCode>(key);

	switch (action)
	{
	case GLFW_PRESS:
	{
		KeyPressedEvent event(keyCode, false);
		win.m_Callback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		KeyReleasedEvent event(keyCode);
		win.m_Callback(event);
		break;
	}
	case GLFW_REPEAT:
	{
		KeyPressedEvent event(keyCode, true);
		win.m_Callback(event);
		break;
	}
	default: SND_ERROR("Unknown key action \'{}\'", action);
	}
}

void snd::WindowsWindow::OnSetChar(GLFWwindow* window, unsigned int key)
{
	WindowsWindow& win = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
	const KeyCode keyCode = static_cast<KeyCode>(key);
	KeyTypedEvent event(keyCode);
	win.m_Callback(event);
}

void snd::WindowsWindow::OnMouseMove(GLFWwindow* window, double xPos, double yPos)
{
	WindowsWindow& win = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
	MouseMovedEvent event(static_cast<f32>(xPos), static_cast<f32>(yPos));
	win.m_Callback(event);
}

void snd::WindowsWindow::OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	WindowsWindow& win = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
	MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
	win.m_Callback(event);
}

void snd::WindowsWindow::OnMouseKey(GLFWwindow* window, int key, int action, int mods)
{
	WindowsWindow& win = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
	const MouseCode mouseCode = static_cast<MouseCode>(key);

	switch (action)
	{
	case GLFW_PRESS:
	{
		MouseKeyPressedEvent event(mouseCode);
		win.m_Callback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		MouseKeyReleasedEvent event(mouseCode);
		win.m_Callback(event);
		break;
	}
	default: SND_ERROR("Unknown mouse key action \'{}\'", action);
	}
}

void snd::WindowsWindow::OnFocus(GLFWwindow* window, int focused)
{
	WindowsWindow& win = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
	win.m_Focused = (focused == GLFW_TRUE);
}
