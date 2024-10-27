#include "sndpch.h"
#include "Engine/Ecs/Systems/MovementSystem.h"
#include "Engine/Ecs/Ecs.h"
#include "Engine/Ecs/Components/MovementComponent.h"
#include "Engine/Ecs/Components/TransformComponent.h"

void snd::TickMovementSystem(f32 dt)
{
    for (Entity e = 0; e < kMaxEntities; ++e)
    {
        auto* transform = (TransformComponent*)gEcs->ComponentData(e, COMPONENT_TRANSFORM);
        auto* movement = (MovementComponent*)gEcs->ComponentData(e, COMPONENT_MOVEMENT);

        transform->Location += movement->Velocity * dt;
    }
}
