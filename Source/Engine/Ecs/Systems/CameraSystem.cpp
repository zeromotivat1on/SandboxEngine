#include "sndpch.h"
#include "Engine/Ecs/Systems/CameraSystem.h"
#include "Engine/Ecs/Ecs.h"
#include "Engine/Ecs/Components/CameraComponent.h"
#include "Engine/Ecs/Components/TransformComponent.h"

void snd::TickCameraSystem(float dt)
{
    for (Entity e = 0; e < kMaxEntities; ++e)
    {
        auto* transform = (TransformComponent*)gEcs->ComponentData(e, COMPONENT_TRANSFORM);
        auto* camera = (CameraComponent*)gEcs->ComponentData(e, COMPONENT_CAMERA);

        camera->Eye = transform->Location;
        camera->At = camera->Eye + ForwardVector(camera->Yaw, camera->Pitch);
    }
}
