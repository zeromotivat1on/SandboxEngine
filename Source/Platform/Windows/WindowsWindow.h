#pragma once

#include "SandboxEngine/Core/Window.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace snd
{
	class WindowsWindow : public Window
	{
	public:
							WindowsWindow(const Props& props);
							NOT_COPYABLE(WindowsWindow);
							NOT_MOVABLE (WindowsWindow);
							~WindowsWindow() override;

		void* 				Handle() const override;
		void* 				NativeHandle() const override;

		u16 				Width() const override;
		u16 				Height() const override;
		const char*			Title() const override;
		bool 				Vsync() const override;
		bool 				Focused() const override;

		void				SetEventCallback(const EventCallback& callback) override;
		void 				SetVsync(bool enable) override;
		bool				ShouldClose() const override;

		void				Update() override;

	private:
		void 				Init(const Props& props);
		void 				Shutdown();

	private:
		static void 		OnClose(GLFWwindow* window);
		static void 		OnResize(GLFWwindow* window, int width, int height);
		static void 		OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void 		OnSetChar(GLFWwindow* window, unsigned int key);
		static void 		OnMouseMove(GLFWwindow* window, double xPos, double yPos);
		static void 		OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
		static void 		OnMouseKey(GLFWwindow* window, int key, int action, int mods);
		static void 		OnFocus(GLFWwindow* window, int focused);

	private:
		GLFWwindow*			m_Window;
		const char*			m_Title;
		u16					m_Width;
		u16					m_Height;
		bool				m_Vsync;
		bool				m_Focused;
		EventCallback		m_Callback;
	};

	SND_INLINE WindowsWindow::WindowsWindow(const Props& props)
		: m_Window(nullptr), m_Width(0), m_Height(0), m_Vsync(true)
	{
		Init(props);
	}

	SND_INLINE WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	SND_INLINE void* WindowsWindow::Handle() const
	{
		return m_Window;
	}

	SND_INLINE void* WindowsWindow::NativeHandle() const
	{
		return glfwGetWin32Window(m_Window);
	}

	SND_INLINE u16 WindowsWindow::Width() const
	{
		return m_Width;
	}

	SND_INLINE u16 WindowsWindow::Height() const
	{
		return m_Height;
	}

	SND_INLINE const char* WindowsWindow::Title() const
	{
		return m_Title;
	}

	SND_INLINE void WindowsWindow::SetEventCallback(const EventCallback& callback)
	{
		m_Callback = callback;
	}

	SND_INLINE bool WindowsWindow::Vsync() const
	{
		return m_Vsync;
	}

	SND_INLINE bool WindowsWindow::Focused() const
	{
		return m_Focused;
	}

	SND_INLINE bool WindowsWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	SND_INLINE void WindowsWindow::SetVsync(bool enable)
	{
		m_Vsync = enable;
	}

	SND_INLINE void WindowsWindow::Update()
	{
		glfwPollEvents();
	}
}
