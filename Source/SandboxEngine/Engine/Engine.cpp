#include "sndpch.h"
#include "SandboxEngine/Engine/Engine.h"
#include "SandboxEngine/Core/Profile.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Render/Render.h"
#include "SandboxEngine/UI/UI.h"
#include "SandboxEngine/Ecs/Ecs.h"
#include "SandboxEngine/Ecs/Systems/CameraSystem.h"
#include "SandboxEngine/Ecs/Systems/MovementSystem.h"
#include "SandboxEngine/Components/CameraComponent.h"
#include "SandboxEngine/Components/MovementComponent.h"
#include "SandboxEngine/Components/TransformComponent.h"

namespace snd
{
	Entity	g_PlayerEntity;
	f32		g_PlayerCameraInputScale = 1.0f;

	// TODO: current test player tick works for perspective camera, for orthographic it's not fully applicable.  
	void TickPlayerInput()
	{
		auto* movement	= ecs::Get<MovementComponent>(g_PlayerEntity);
		auto* camera	= ecs::Get<CameraComponent>(g_PlayerEntity);
		SND_ASSERT(movement && camera);
		
		const vec2 mouseOffset		= input::MouseOffset();
		const f32 mouseSensitivity  = 0.1f; // todo: mb make separate component for inputs
		const f32 pitchLimit        = 89.0f;
        
		camera->Yaw   += mouseOffset.x * mouseSensitivity;
		camera->Pitch += mouseOffset.y * mouseSensitivity;
		camera->Pitch  = std::clamp(camera->Pitch, -pitchLimit, pitchLimit);
		
		vec3 inputVelocity;
		
		if (input::ButtonDown(KeyboardBit::W))
		{
			inputVelocity.x += 50.0f;
		}

		if (input::ButtonDown(KeyboardBit::S))
		{
			inputVelocity.x -= 50.0f;
		}

		if (input::ButtonDown(KeyboardBit::D))
		{
			inputVelocity.y += 50.0f;
		}
		
		if (input::ButtonDown(KeyboardBit::A))
		{
			inputVelocity.y -= 50.0f;
		}
		
		if (input::ButtonDown(KeyboardBit::E))
		{
			inputVelocity.z += 50.0f;
		}

		if (input::ButtonDown(KeyboardBit::Q))
		{
			inputVelocity.z -= 50.0f;
		}

		movement->Velocity =
			g_PlayerCameraInputScale * inputVelocity.x * camera->ForwardVector() +
			g_PlayerCameraInputScale * inputVelocity.y * camera->RightVector()	 +
			g_PlayerCameraInputScale * inputVelocity.z * camera->Up;
	}
}

snd::Engine::Engine()
	: m_Window(nullptr), m_ExitRequested(false)
{
}

bool snd::Engine::Running() const
{
	return !m_ExitRequested;
}

void snd::Engine::Init(Window* window)
{
	SND_SCOPE_FUNCTION_TIMER();

	SND_INFO("Initializing the engine");
	SND_ASSERT(window);
	
	m_Window = window;
	m_Window->SetEventCallback(SND_BIND_EVENT_FN(Engine::OnEvent));

	ecs::Init();
	input::Init(m_Window);
	render::Init(m_Window);
	//ui::Init(m_Window);
	
	g_PlayerEntity = ecs::NewEntity();

	auto* transform = ecs::Assign<TransformComponent>(g_PlayerEntity, IdentityTransform());
	transform->Location = vec3(0.0f, 0.0f, -50.0f);

	const vec4 ortho = m_Window->OrthoDataCentered();
	auto* camera = ecs::Assign<CameraComponent>(g_PlayerEntity);
	camera->Eye  	= transform->Location;
	camera->At	 	= vec3(0.0f);
	camera->Up		= vec3(0.0f, 1.0f, 0.0f);
	camera->Yaw		= 0.0f;
	camera->Pitch	= 0.0f;
	camera->Fov		= 60.0f;
	camera->Aspect	= m_Window->AspectRatio();
	camera->Near	= 0.1f;
	camera->Far		= 1000.0f;
	camera->Left	= ortho[0] * 0.1f;
	camera->Right	= ortho[1] * 0.1f;
	camera->Bottom	= ortho[2] * 0.1f;
	camera->Top		= ortho[3] * 0.1f;

	auto* movement = ecs::Assign<MovementComponent>(g_PlayerEntity);
	movement->Velocity = vec3(0.0f);
	
	render::SetCamera(camera);
}

void snd::Engine::Shutdown()
{
	SND_INFO("Shutting down the engine");

	input::Shutdown();
	//ui::Shutdown();
	render::Shutdown();
	ecs::Shutdown();
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
	if (auto* camera = ecs::Get<CameraComponent>(g_PlayerEntity))
	{
		const vec4 ortho = m_Window->OrthoDataCentered();
		camera->Left	 = ortho[0] * 0.1f;
		camera->Right	 = ortho[1] * 0.1f;
		camera->Bottom	 = ortho[2] * 0.1f;
		camera->Top		 = ortho[3] * 0.1f;
	}
	
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
	g_PlayerCameraInputScale += math::Sign(event.GetOffsetY()) * 0.2f;
	g_PlayerCameraInputScale  = std::clamp(g_PlayerCameraInputScale, 0.1f, 10.0f);
	
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

	TickPlayerInput();
	
	ecs::TickMovementSystem(dt);
	ecs::TickCameraSystem(dt);
	
	render::Tick(dt);

	//ui::Tick(dt);
}
