#include "sndpch.h"
#include "SandBoxEngine/Ecs/Systems/CameraSystem.h"
#include "SandboxEngine/Ecs/Ecs.h"
#include "SandboxEngine/Ecs/EntityFilter.h"
#include "SandboxEngine/Components/CameraComponent.h"
#include "SandboxEngine/Components/TransformComponent.h"

void snd::ecs::TickCameraSystem(float dt)
{
    for (Entity entity : EntityFilterTemplate<TransformComponent, CameraComponent>())
    {
        auto* transform = ecs::Get<TransformComponent>(entity);
        auto* camera    = ecs::Get<CameraComponent>(entity);
        SND_ASSERT(transform && camera);

        camera->Eye = transform->Location;
        camera->At  = camera->Eye + ForwardVector(camera->Yaw, camera->Pitch);
    }
}
