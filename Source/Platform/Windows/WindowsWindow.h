#pragma once

#include "SandboxEngine/Core/Window.h"
#include <GLFW/glfw3.h>

namespace snd
{
	class WindowsWindow : public Window
	{
		struct Data
		{
			std::string Title = "";
			uint32_t Width = 0;
			uint32_t Height = 0;
			bool Vsync = false;
			EventCallbackFn EventCallback;
		};

	public:
		WindowsWindow(const Window::Props& props);
		virtual ~WindowsWindow();

		void* GetNativeHandle() const override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }
		inline const char* GetTitle() const override { return m_Data.Title.c_str(); }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		bool IsVsync() const override;
		void SetVsync(bool enable) override;

		bool ShouldClose() const override;

		void OnUpdate() const override;

	private:
		void Init(const Window::Props& props);
		void Shutdown();

	private:
		static void OnClose(GLFWwindow* window);
		static void OnResize(GLFWwindow* window, int width, int height);

	private:
		GLFWwindow* m_Window = nullptr;
		Data m_Data = {};
	};
}
