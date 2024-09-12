#include "sndpch.h"
#include "SandboxEngine/Engine/Engine.h"
#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Core/Profile.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Render/Render.h"
#include "SandboxEngine/World/Camera.h"
#include "SandboxEngine/UI/UI.h"

snd::Camera g_DebugCamera = snd::Camera(snd::Camera::Type::Perspective);

snd::Engine::Engine(std::unique_ptr<Window>&& window)
	: m_Window(std::move(window))
{
	Init();
}

snd::Engine::~Engine()
{
	Shutdown();
}

bool snd::Engine::Running() const
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

	g_DebugCamera.SetLocation(glm::vec3(0.0f, 0.0f, -35.0f));
	g_DebugCamera.SetTarget(glm::vec3(0.0f));
	g_DebugCamera.SetPerspective(60.0f, m_Window->GetAspectRatio(), 0.1f, 1000.0f);

	render::SetCamera(&g_DebugCamera);
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
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowClosedEvent>		(SND_BIND_EVENT_FN(Engine::OnWindowClosed));
	dispatcher.Dispatch<WindowResizedEvent>		(SND_BIND_EVENT_FN(Engine::OnWindowResized));
	dispatcher.Dispatch<KeyPressedEvent>		(SND_BIND_EVENT_FN(Engine::OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>		(SND_BIND_EVENT_FN(Engine::OnKeyReleased));
	dispatcher.Dispatch<KeyTypedEvent>			(SND_BIND_EVENT_FN(Engine::OnKeyTyped));
	dispatcher.Dispatch<MouseMovedEvent>		(SND_BIND_EVENT_FN(Engine::OnMouseMoved));
	dispatcher.Dispatch<MouseScrolledEvent>		(SND_BIND_EVENT_FN(Engine::OnMouseScrolled));
	dispatcher.Dispatch<MouseKeyPressedEvent>	(SND_BIND_EVENT_FN(Engine::OnMouseKeyPressed));
	dispatcher.Dispatch<MouseKeyReleasedEvent>	(SND_BIND_EVENT_FN(Engine::OnMouseKeyReleased));
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
	return true;
}

bool snd::Engine::OnKeyReleased(KeyReleasedEvent& event)
{
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
	m_Window->Update();

	input::Update();
	
	if (input::ButtonJustWentUp(KeyboardBit::Escape))
	{
		SND_LOG_TRACE("Manual shutdown was requested", m_Window->GetTitle());
		m_ExitRequested = true;
	}
	
	g_DebugCamera.Tick(dt);
	
	render::Tick(dt);

	ui::Tick(dt);
}
