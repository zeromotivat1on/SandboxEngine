#pragma once

#include "SandboxEngine/Core/CoreIncludes.h"
#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Events/WindowEvent.h"
#include "SandboxEngine/Events/KeyEvent.h"
#include "SandboxEngine/Events/MouseEvent.h"

namespace snd
{
	class Engine
	{
	public:
		Engine(std::unique_ptr<Window>&& window);
		~Engine();

		bool Running() const;

		void Tick(float dt);

	private:
		void Init();
		void Shutdown();

		void OnEvent(Event& event);
		bool OnWindowClosed(WindowClosedEvent& event);
		bool OnWindowResized(WindowResizedEvent& event);
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
		bool OnKeyTyped(KeyTypedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnMouseKeyPressed(MouseKeyPressedEvent& event);
		bool OnMouseKeyReleased(MouseKeyReleasedEvent& event);

	private:
		std::unique_ptr<Window> m_Window = nullptr;
		bool m_ExitRequested = false;
	};
}
