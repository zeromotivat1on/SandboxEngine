#include "sndpch.h"
#include "SandboxEngine/Engine/Engine.h"
#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Core/Profile.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Render/Render.h"
#include "SandboxEngine/UI/UI.h"
#include "SandboxEngine/Ecs/EntityContainer.h"
#include "SandboxEngine/Components/CameraComponent.h"

snd::EntityId g_PlayerEntity;

snd::Engine::Engine(Window* window)
	: m_Window(window), m_ExitRequested(false)
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
	SND_SCOPE_FUNCTION_TIMER();
	SND_INFO("Initializing the engine");

	m_Window->SetEventCallback(SND_BIND_EVENT_FN(Engine::OnEvent));

	input::Init(m_Window);
	render::Init(m_Window);
	ui::Init(m_Window);

	g_PlayerEntity = ecs::NewEntity();

	auto* camera = ecs::Assign<CameraComponent>(g_PlayerEntity);
	camera->Eye  	= glm::vec3(0.0f, 0.0f, -35.0f);
	camera->At	 	= glm::vec3(0.0f);
	camera->Up		= glm::vec3(0.0f, 1.0f, 0.0f);
	camera->Yaw		= 0.0f;
	camera->Pitch	= 0.0f;
	camera->Fov		= 60.0f;
	camera->Aspect	= m_Window->GetAspectRatio();
	camera->Near	= 0.1f;
	camera->Far		= 1000.0f;
	
	render::SetCamera(camera);
}

void snd::Engine::Shutdown()
{
	SND_INFO("Shutting down the engine");

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
	SND_TRACE("Window \"{}\" was manually closed, shutting down", m_Window->Title());
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

void snd::Engine::Tick(f32 dt)
{
	m_Window->Update();

	if (!m_Window->Focused())
	{
		return;
	}
	
	input::Update();
	
	if (input::ButtonJustWentUp(KeyboardBit::Escape))
	{
		SND_TRACE("Manual shutdown was requested", m_Window->Title());
		m_ExitRequested = true;
	}

	// Update camera controls
	if (auto* camera = ecs::Get<CameraComponent>(g_PlayerEntity))
	{
		camera->OnMouseMove(input::MouseOffset(), 0.1f);

		glm::vec3 moveDelta = glm::vec3(0.0f);

		if (input::ButtonDown(KeyboardBit::W))
		{
			moveDelta.x += 1.0f;
		}
		
		if (input::ButtonDown(KeyboardBit::S))
		{
			moveDelta.x -= 1.0f;
		}

		if (input::ButtonDown(KeyboardBit::D))
		{
			moveDelta.y += 1.0f;
		}
		
		if (input::ButtonDown(KeyboardBit::A))
		{
			moveDelta.y -= 1.0f;
		}

		if (input::ButtonDown(KeyboardBit::E))
		{
			moveDelta.z += 1.0f;
		}
		
		if (input::ButtonDown(KeyboardBit::Q))
		{
			moveDelta.z -= 1.0f;
		}

		camera->Move(moveDelta);
	}
	
	render::Tick(dt);

	ui::Tick(dt);
}
