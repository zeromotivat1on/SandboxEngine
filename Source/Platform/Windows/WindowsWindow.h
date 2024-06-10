#pragma once

#include "SandboxEngine/Window.h"
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
		};

	public:
		WindowsWindow(const Window::Props& props);
		virtual ~WindowsWindow();

		void* GetNativeHandle() const override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		bool IsVsync() const override;
		void SetVsync(bool enable) override;

		bool ShouldClose() const override;

		void OnUpdate() const override;

	private:
		void Init(const Window::Props& props);
		void Shutdown();

	private:
		GLFWwindow* m_Window = nullptr;
		Data m_Data = {};
	};
}
