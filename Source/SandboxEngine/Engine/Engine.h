#pragma once

#include "SandboxEngine/Core/Core.h"
#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Events/WindowEvent.h"

namespace snd
{
	class Engine
	{
	public:
		Engine(std::unique_ptr<Window>&& window);
		~Engine();

		bool IsRunning() const;

		void Tick(float dt);

	private:
		void Init();
		void Shutdown();

		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		std::unique_ptr<Window> m_Window = nullptr;
	};
}
