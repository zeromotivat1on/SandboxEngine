#include "sndpch.h"
#include "SandBoxEngine/Ecs/Systems/MovementSystem.h"
#include "SandboxEngine/Ecs/Ecs.h"
#include "SandboxEngine/Ecs/EntityFilter.h"
#include "SandboxEngine/Components/MovementComponent.h"
#include "SandboxEngine/Components/TransformComponent.h"

void snd::TickMovementSystem(f32 dt)
{
    for (Entity entity : EntityFilterTemplate<TransformComponent, MovementComponent>())
    {
        auto* transform = GetComponent<TransformComponent>(entity);
        auto* movement  = GetComponent<MovementComponent>(entity);
        SND_ASSERT(transform && movement);

        transform->Location += movement->Velocity * dt;
    }
}
