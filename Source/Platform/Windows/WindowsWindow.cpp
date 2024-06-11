#include "sndpch.h"
#include "WindowsWindow.h"
#include "SandboxEngine/Events/WindowEvent.h"
#include <GLFW/glfw3native.h>

static uint8_t s_GlfwWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
	SND_LOG_ERROR("GLFW Error {}: {}", error, description);
}

snd::Window* snd::Window::Create(const snd::Window::Props& props /*= {}*/)
{
	return new WindowsWindow(props);
}

snd::WindowsWindow::WindowsWindow(const Window::Props& props)
{
	Init(props);
}

snd::WindowsWindow::~WindowsWindow()
{
	Shutdown();
}

void* snd::WindowsWindow::GetNativeHandle() const
{
	return glfwGetWin32Window(m_Window);
}

void snd::WindowsWindow::Init(const Window::Props& props)
{
	if (s_GlfwWindowCount == 0)
	{
		const int success = glfwInit();
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

	s_GlfwWindowCount++;

	glfwSetWindowCloseCallback(m_Window, WindowsWindow::OnClose);
	glfwSetWindowSizeCallback(m_Window, WindowsWindow::OnResize);
}

void snd::WindowsWindow::Shutdown()
{
	glfwDestroyWindow(m_Window);
	s_GlfwWindowCount--;

	if (s_GlfwWindowCount == 0)
	{
		glfwTerminate();
	}
}

void snd::WindowsWindow::OnClose(GLFWwindow* window)
{
	Data& data = *(Data*)glfwGetWindowUserPointer(window);

	WindowCloseEvent event;
	data.EventCallback(event);
}

void snd::WindowsWindow::OnResize(GLFWwindow* window, int width, int height)
{
	Data& data = *(Data*)glfwGetWindowUserPointer(window);
	data.Width = width;
	data.Height = height;

	WindowResizeEvent event(width, height);
	data.EventCallback(event);
}

bool snd::WindowsWindow::IsVsync() const
{
	return m_Data.Vsync;
}

void snd::WindowsWindow::SetVsync(bool enable)
{
	m_Data.Vsync = enable;
}

bool snd::WindowsWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void snd::WindowsWindow::OnUpdate() const
{
	glfwPollEvents();
}
