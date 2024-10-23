#include "sndpch.h"
#include "SandboxEngine/Core/EntryPoint.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Render/Render.h"
#include "SandboxEngine/Filesystem/Filesystem.h"
#include "SandboxEngine/Ecs/Ecs.h"

// Temp test player and stuff.
#include "Components/TransformComponent.h"
#include "Components/MovementComponent.h"
#include "Components/CameraComponent.h"
#include "SandboxEngine/Ecs/Systems/CameraSystem.h"
#include "SandboxEngine/Ecs/Systems/MovementSystem.h"

namespace snd
{
	f32 gCameraMoveInputScale = 1.0f;

	Entity InitTestPlayer(Window* wnd)
	{
		Entity player = NewEntity();

		auto* transform = AssignComponent<TransformComponent>(player, IdentityTransform());
		transform->Location = vec3(0.0f, 0.0f, -50.0f);

		const vec4 ortho = wnd->OrthoDataCentered();
		auto* camera = AssignComponent<CameraComponent>(player);
		camera->Eye  	= transform->Location;
		camera->At	 	= vec3(0.0f);
		camera->Up		= vec3(0.0f, 1.0f, 0.0f);
		camera->Yaw		= 0.0f;
		camera->Pitch	= 0.0f;
		camera->Fov		= 60.0f;
		camera->Aspect	= wnd->AspectRatio();
		camera->Near	= 0.1f;
		camera->Far		= 1000.0f;
		camera->Left	= ortho[0] * 0.1f;
		camera->Right	= ortho[1] * 0.1f;
		camera->Bottom	= ortho[2] * 0.1f;
		camera->Top		= ortho[3] * 0.1f;

		auto* movement = AssignComponent<MovementComponent>(player);
		movement->Velocity = vec3(0.0f);

		return player;
	}

	void TickTestPlayer(Entity player, Entity cube, Window* wnd, f32 dt)
	{
		if (wnd->ButtonsDown[INPUT_KEY_ESCAPE])
    	{
    		SND_LOG("Manual shutdown is requested");
    		wnd->ShouldClose(true);
    		return;
    	}

        if (wnd->ButtonsDown[INPUT_KEY_F])
        {
            SND_LOG("Centering on given cube entity");

            auto* playerTransform = GetComponent<TransformComponent>(player);
            auto* cubeTransform = GetComponent<TransformComponent>(cube);

            if (playerTransform && cubeTransform)
            {
                playerTransform->Location = cubeTransform->Location;
            }

            return;
        }

		gCameraMoveInputScale += math::Sign(wnd->Axes[INPUT_MOUSE_SCROLL_Y]) * 0.2f;
		gCameraMoveInputScale  = std::clamp(gCameraMoveInputScale, 0.1f, 10.0f);

		auto* movement	= GetComponent<MovementComponent>(player);
		auto* camera	= GetComponent<CameraComponent>(player);
		SND_ASSERT(movement && camera);

		const f32 mouseSensitivity  = 0.1f; // todo: mb make separate component for inputs
		const f32 pitchLimit        = 89.0f;

		camera->Yaw   += wnd->Axes[INPUT_MOUSE_OFFSET_X] * mouseSensitivity;
		camera->Pitch += wnd->Axes[INPUT_MOUSE_OFFSET_Y] * mouseSensitivity;
		camera->Pitch  = std::clamp(camera->Pitch, -pitchLimit, pitchLimit);

        const f32 moveSpeed = 3000.0f * gCameraMoveInputScale;
		vec3 inputVelocity;

		if (wnd->Buttons[INPUT_KEY_W])
		{
			inputVelocity.x += moveSpeed * dt;
		}

		if (wnd->Buttons[INPUT_KEY_S])
		{
			inputVelocity.x -= moveSpeed * dt;
		}

		if (wnd->Buttons[INPUT_KEY_D])
		{
			inputVelocity.y += moveSpeed * dt;
		}

		if (wnd->Buttons[INPUT_KEY_A])
		{
			inputVelocity.y -= moveSpeed * dt;
		}

		if (wnd->Buttons[INPUT_KEY_E])
		{
			inputVelocity.z += moveSpeed * dt;
		}

		if (wnd->Buttons[INPUT_KEY_Q])
		{
			inputVelocity.z -= moveSpeed * dt;
		}

		movement->Velocity =
			inputVelocity.x * camera->ForwardVector() +
			inputVelocity.y * camera->RightVector()	+
			inputVelocity.z * camera->Up;
	}
}

s32 snd::EntryPoint()
{
    gLogStdout = GetStdout();
    gHighPrecisionFrequency = HighPrecisionFrequency();

    OsInfo osinfo;
    GetOsInfo(&osinfo);
    SND_ASSERT(osinfo.AvailVirt > VM_CORE_ALLOC_SIZE);

#if SND_BUILD_DEBUG
    void* baseAddr = (void*)TB(2);
#else
    void* baseAddr = nullptr;
#endif
    void* vm = vmreserve(baseAddr, VM_CORE_ALLOC_SIZE);
    InitAppMemory(vm, VM_CORE_ALLOC_SIZE);

    InitCorePaths();
    InitInputMaps();

    gWindowCount = 0;
    gWindow = OpenWindow("Sandbox Engine", 1280, 720);
    gWindow->EnableCursor(true);

    gEntityContainer = (EntityContainer*)gPersistentStack.PushZero(sizeof(EntityContainer));
    Entity player = InitTestPlayer(gWindow);

    gRenderer = (Renderer*)gPersistentStack.PushZero(sizeof(Renderer));
    gRenderer->Window = gWindow;
    gRenderer->Camera = GetComponent<CameraComponent>(player);
    gRenderer->Vsync = true;
    gRenderer->Init();

	// Create debug test cubes.
	// TODO: vector in entity container is throwing exception on debug build, dont know why,
	// also need to get rid of stl as much as possible in ecs and other engine parts.
    Entity cube;
    for (s32 i = 0; i < 4; ++i)
    {
		cube = NewEntityDebugCube();
		GetComponent<TransformComponent>(cube)->Location.x += i * 10.0f;
    }

	f32 dt = FRAMERATE(60);
	u64 beginCounter = HighPrecisionCounter();
	while (!gWindow->ShouldClose())
	{
        gFrameStack.Clear();

        gWindow->Update();

        TickTestPlayer(player, cube, gWindow, dt);
        TickMovementSystem(dt);
        TickCameraSystem(dt);

        gRenderer->Render(dt);

		const u64 endCounter = HighPrecisionCounter();
		dt = (f32)(endCounter - beginCounter) / (f32)gHighPrecisionFrequency;
		beginCounter = endCounter;

#ifdef SND_BUILD_DEBUG
		// If dt is too large, we must have resumed from a breakpoint, lock to the target framerate.
		if (dt > 1.0f)
		{
			dt = FRAMERATE(60);
		}
#endif
	}

	gRenderer->Terminate();
	CloseWindow(gWindow);
    FreeAppMemory(vm);

	return SND_CORE_SUCCESS;
}
