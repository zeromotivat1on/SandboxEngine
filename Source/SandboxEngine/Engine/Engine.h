#pragma once

#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Events/KeyEvent.h"
#include "SandboxEngine/Events/MouseEvent.h"
#include "SandboxEngine/Events/WindowEvent.h"

namespace snd
{
	class Engine
	{
	public:
						Engine();

		void			Init(Window* window);
		void			Shutdown();

		void			Tick(f32 dt);
		bool			Running() const;

	private:
		void			OnEvent(Event& event);
		bool			OnWindowClosed(WindowClosedEvent& event);
		bool			OnWindowResized(WindowResizedEvent& event);
		bool			OnKeyPressed(KeyPressedEvent& event);
		bool			OnKeyReleased(KeyReleasedEvent& event);
		bool			OnKeyTyped(KeyTypedEvent& event);
		bool			OnMouseMoved(MouseMovedEvent& event);
		bool			OnMouseScrolled(MouseScrolledEvent& event);
		bool			OnMouseKeyPressed(MouseKeyPressedEvent& event);
		bool			OnMouseKeyReleased(MouseKeyReleasedEvent& event);

	private:
		Window*			m_Window;
		bool			m_ExitRequested;
	};
}
