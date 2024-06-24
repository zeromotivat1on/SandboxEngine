#include "sndpch.h"
#include "SandboxEngine/Engine/Engine.h"
#include "SandboxEngine/UI/UI.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Render/Render.h"

snd::Engine::Engine(std::unique_ptr<Window>&& window)
	: m_Window(std::move(window))
{
	Init();
}

snd::Engine::~Engine()
{
	Shutdown();
}

bool snd::Engine::IsRunning() const
{
	return !m_ExitRequested;
}

void snd::Engine::Init()
{
	SND_SCOPE_TIMER(__FUNCTION__);
	SND_LOG_INFO("Initializing the engine");

	m_Window->SetEventCallback(SND_BIND_EVENT_FN(Engine::OnEvent));

	input::Init(m_Window.get());
	render::Init(m_Window.get());
	ui::Init(m_Window.get());
}

void snd::Engine::Shutdown()
{
	SND_LOG_INFO("Shutting down the engine");

	input::Shutdown();
	ui::Shutdown();
	render::Shutdown();
}

void snd::Engine::OnEvent(Event& event)
{
#define DISPATCH_EVENT(eventType) dispatcher.Dispatch<eventType##Event>(SND_BIND_EVENT_FN(Engine::On##eventType))

	EventDispatcher dispatcher(event);
	DISPATCH_EVENT(WindowClosed);
	DISPATCH_EVENT(WindowResized);
	DISPATCH_EVENT(KeyPressed);
	DISPATCH_EVENT(KeyReleased);
	DISPATCH_EVENT(KeyTyped);
	DISPATCH_EVENT(MouseMoved);
	DISPATCH_EVENT(MouseScrolled);
	DISPATCH_EVENT(MouseKeyPressed);
	DISPATCH_EVENT(MouseKeyReleased);

#undef DISPATCH_EVENT
}

bool snd::Engine::OnWindowClosed(WindowClosedEvent& event)
{
	SND_LOG_TRACE("Window \"{}\" was manually closed, shutting down", m_Window->GetTitle());
	m_ExitRequested = true;
	return true;
}

bool snd::Engine::OnWindowResized(WindowResizedEvent& event)
{
	render::OnWindowResized(event.GetWidth(), event.GetHeight());
	return true;
}

bool snd::Engine::OnKeyPressed(KeyPressedEvent& event)
{
	switch (event.GetKeyCode())
	{
	case KeyCode::A:
		render::GetCamera().MoveLeft(1.0f); break;
	case KeyCode::D:
		render::GetCamera().MoveRight(1.0f); break;
	case KeyCode::W:
		render::GetCamera().MoveForward(1.0f); break;
	case KeyCode::S:
		render::GetCamera().MoveBackward(1.0f); break;
	case KeyCode::Left:
		render::GetCamera().RotateLeft(1.0f); break;
	case KeyCode::Right:
		render::GetCamera().RotateRight(1.0f); break;
	case KeyCode::Up:
		render::GetCamera().RotateUp(1.0f); break;
	case KeyCode::Down:
		render::GetCamera().RotateDown(1.0f); break;
	}

	return true;
}

bool snd::Engine::OnKeyReleased(KeyReleasedEvent& event)
{
	if (event.GetKeyCode() == KeyCode::Escape)
	{
		SND_LOG_TRACE("Manual shutdown was requested", m_Window->GetTitle());
		m_ExitRequested = true;
	}

	return true;
}

bool snd::Engine::OnKeyTyped(KeyTypedEvent& event)
{
	return true;
}

bool snd::Engine::OnMouseMoved(MouseMovedEvent& event)
{
	return true;
}

bool snd::Engine::OnMouseScrolled(MouseScrolledEvent& event)
{
	return true;
}

bool snd::Engine::OnMouseKeyPressed(MouseKeyPressedEvent& event)
{
	return true;
}

bool snd::Engine::OnMouseKeyReleased(MouseKeyReleasedEvent& event)
{
	return true;
}

void snd::Engine::Tick(float dt)
{
	m_Window->Tick(dt);
	ui::Tick(dt);
	render::Tick(dt);
}
