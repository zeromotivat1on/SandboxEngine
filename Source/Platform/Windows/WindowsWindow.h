#pragma once

#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Core/Window.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace snd
{
	class WindowsWindow : public Window
	{
	public:
		struct Data
		{
			std::string		Title;
			u16				Width;
			u16				Height;
			bool			Vsync;
			EventCallback	Callback;
		};

	public:
							WindowsWindow(const Props& props);
							NOT_COPYABLE(WindowsWindow);
							NOT_MOVABLE (WindowsWindow);
							~WindowsWindow() override;

		void* 				GetHandle() const override;
		void* 				GetNativeHandle() const override;

		u16 				GetWidth() const override;
		u16 				GetHeight() const override;
		const char*			GetTitle() const override;

		void				SetEventCallback(const EventCallback& callback) override;

		bool 				IsVsync() const override;
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

	private:
		GLFWwindow*			m_Window;
		Data				m_Data;
	};

	SND_INLINE WindowsWindow::WindowsWindow(const Props& props)
		: m_Window(nullptr), m_Data()
	{
		Init(props);
	}

	SND_INLINE WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}
	
	SND_INLINE void* WindowsWindow::GetHandle() const
	{
		return m_Window;
	}

	SND_INLINE void* WindowsWindow::GetNativeHandle() const
	{
		return glfwGetWin32Window(m_Window);
	}
	
	SND_INLINE u16 WindowsWindow::GetWidth() const
	{
		return m_Data.Width;
	}

	SND_INLINE u16 WindowsWindow::GetHeight() const
	{
		return m_Data.Height;
	}

	SND_INLINE const char* WindowsWindow::GetTitle() const
	{
		return m_Data.Title.c_str();
	}

	SND_INLINE void WindowsWindow::SetEventCallback(const EventCallback& callback)
	{
		m_Data.Callback = callback;
	}

	SND_INLINE bool WindowsWindow::IsVsync() const
	{
		return m_Data.Vsync;
	}

	SND_INLINE void WindowsWindow::SetVsync(bool enable)
	{
		m_Data.Vsync = enable;
	}

	SND_INLINE bool WindowsWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	SND_INLINE void WindowsWindow::Update()
	{
		glfwPollEvents();
	}

}
