#include "sndpch.h"
#include "Engine/Core/EntryPoint.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Window.h"
#include "Engine/Render/Render.h"
#include "Engine/Filesystem/Filesystem.h"
#include "Engine/Ecs/Ecs.h"

// Temp test player and stuff.
#include "Ecs/Components/TransformComponent.h"
#include "Ecs/Components/MovementComponent.h"
#include "Ecs/Components/CameraComponent.h"

namespace snd
{
	f32 gCameraMoveInputScale = 1.0f;

	Entity InitTestPlayer(Window* wnd)
	{
		Entity player = gEcs->NewEntity();

		auto* transform = new (gEcs->ComponentData(player, COMPONENT_TRANSFORM)) TransformComponent(IdentityTransform());
		transform->Location = vec3(0.0f, 0.0f, -50.0f);

		const vec4 ortho = wnd->OrthoDataCentered();
		auto* camera = (CameraComponent*)gEcs->ComponentData(player, COMPONENT_CAMERA);
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

		auto* movement = (MovementComponent*)gEcs->ComponentData(player, COMPONENT_MOVEMENT);
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

            auto* playerTransform = (TransformComponent*)gEcs->ComponentData(player, COMPONENT_TRANSFORM);
            auto* cubeTransform = (TransformComponent*)gEcs->ComponentData(cube, COMPONENT_TRANSFORM);

            if (playerTransform && cubeTransform)
            {
                playerTransform->Location = cubeTransform->Location;
            }

            return;
        }

        if (wnd->ButtonsDown[INPUT_KEY_1])
        {
            gEcs->DeleteEntity(1);
        }
        if (wnd->ButtonsDown[INPUT_KEY_2])
        {
            gEcs->DeleteEntity(2);
        }
        if (wnd->ButtonsDown[INPUT_KEY_3])
        {
            gEcs->DeleteEntity(3);
        }

		gCameraMoveInputScale += math::Sign(wnd->Axes[INPUT_MOUSE_SCROLL_Y]) * 0.2f;
		gCameraMoveInputScale  = std::clamp(gCameraMoveInputScale, 0.1f, 10.0f);

		auto* movement = (MovementComponent*)gEcs->ComponentData(player, COMPONENT_MOVEMENT);
		auto* camera = (CameraComponent*)gEcs->ComponentData(player, COMPONENT_CAMERA);

		const f32 mouseSensitivity = 0.1f; // todo: mb make separate component for inputs
		const f32 pitchLimit = 89.0f;

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
    gMsgStdout = GetStdout();
    gHighPrecisionFrequency = HighPrecisionFrequency();

#if SND_BUILD_DEBUG
    void* baseAddr = (void*)TB(2);
#else
    void* baseAddr = nullptr;
#endif

    gVirtSpace = ReserveVirtSpace(baseAddr, VIRT_SPACE_SIZE);
    gPhysHeap = AllocPhysHeap(gVirtSpace, PHYS_HEAP_SIZE);

    InitArena(gPersArena, gPhysHeap, PERS_ARENA_SIZE);
    InitArena(gTranArena, gPhysHeap, TRAN_ARENA_SIZE, PERS_ARENA_SIZE);
    InitArena(gFrameArena, gPhysHeap, FRAME_ARENA_SIZE, PERS_ARENA_SIZE + TRAN_ARENA_SIZE);

    InitCorePaths();
    InitInputMaps();

    gWindowCount = 0;
    gWindow = OpenWindow("Sandbox Engine", 1280, 720);
    gWindow->EnableCursor(true);

    gEcs = PushStruct(gPersArena, Ecs);
    gEcs->Init();

    Entity player = InitTestPlayer(gWindow);

    gRenderer = PushStruct(gPersArena, Renderer);
    gRenderer->Window = gWindow;
    gRenderer->Camera = (CameraComponent*)gEcs->ComponentData(player, COMPONENT_CAMERA);
    gRenderer->Vsync = true;
    gRenderer->Init();

	// Create debug test cubes.
    Entity cube;
    for (s32 i = 0; i < 10; ++i)
    {
		cube = NewEntityDebugCube();
		((TransformComponent*)gEcs->ComponentData(cube, COMPONENT_TRANSFORM))->Location.x += i * 10.0f;
    }

	f32 dt = FPS(60);
	u64 beginCounter = HighPrecisionCounter();
	while (!gWindow->ShouldClose())
	{
        gFrameArena.Clear();

        gWindow->Update();
        TickTestPlayer(player, cube, gWindow, dt);
        gEcs->Tick(dt);
        gRenderer->Render(dt);

		const u64 endCounter = HighPrecisionCounter();
		dt = (f32)(endCounter - beginCounter) / (f32)gHighPrecisionFrequency;
		beginCounter = endCounter;

#ifdef SND_BUILD_DEBUG
		// If dt is too large, we must have resumed from a breakpoint, lock to the target framerate.
		if (dt > 1.0f)
		{
			dt = FPS(60);
		}
#endif
	}

	gRenderer->Terminate();
	CloseWindow(gWindow);
    FreePhysHeap(gPhysHeap, PHYS_HEAP_SIZE);
    ReleaseVirtSpace(gVirtSpace);

	return SND_CORE_SUCCESS;
}
