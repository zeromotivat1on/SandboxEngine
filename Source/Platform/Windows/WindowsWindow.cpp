#include "sndpch.h"
#include "WindowsWindow.h"
#include <GLFW/glfw3native.h>

static uint8_t s_GlfwWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
	SND_ERROR("GLFW Error {}: {}", error, description);
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

	SND_INFO("Creating window \"{}\" {}x{}", props.Title, props.Width, props.Height);

	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);

	SetVsync(true);

	s_GlfwWindowCount++;
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

bool snd::WindowsWindow::IsVsync() const
{
	return m_Data.Vsync;
}

void snd::WindowsWindow::SetVsync(bool enable)
{
	m_Data.Vsync = enable;

	// TODO: Should be called only if rendering API is OpenGL / OpenGL ES
	glfwSwapInterval(int(enable));
}

bool snd::WindowsWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void snd::WindowsWindow::OnUpdate() const
{
	glfwPollEvents();

	// TODO: Should be called only if rendering API is OpenGL / OpenGL ES
	glfwSwapBuffers(m_Window);
}
