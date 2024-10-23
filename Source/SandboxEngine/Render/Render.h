#pragma once

#include "SandboxEngine/Ecs/Entity.h"

namespace snd
{
    struct Window;
    struct CameraComponent;

    struct Renderer
    {
        Window*             Window;
        CameraComponent*    Camera;
        bool                Vsync;

        void Init();
        void Terminate();

        void Render(f32 dt);
        void Reset(u16 width, u16 height);
    };

    inline Renderer* gRenderer;

	// Create debug cube entity with default transform.
	Entity NewEntityDebugCube();
}
