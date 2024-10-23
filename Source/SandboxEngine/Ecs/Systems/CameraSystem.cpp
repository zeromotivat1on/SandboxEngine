#include "sndpch.h"
#include "SandBoxEngine/Ecs/Systems/CameraSystem.h"
#include "SandboxEngine/Ecs/Ecs.h"
#include "SandboxEngine/Ecs/EntityFilter.h"
#include "SandboxEngine/Components/CameraComponent.h"
#include "SandboxEngine/Components/TransformComponent.h"

void snd::TickCameraSystem(float dt)
{
    for (Entity entity : EntityFilterTemplate<TransformComponent, CameraComponent>())
    {
        auto* transform = GetComponent<TransformComponent>(entity);
        auto* camera    = GetComponent<CameraComponent>(entity);
        SND_ASSERT(transform && camera);

        camera->Eye = transform->Location;
        camera->At  = camera->Eye + ForwardVector(camera->Yaw, camera->Pitch);
    }
}
