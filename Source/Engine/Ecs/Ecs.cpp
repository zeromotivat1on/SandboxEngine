#include "sndpch.h"
#include "Engine/Ecs/Ecs.h"
#include "Engine/Ecs/Components/TransformComponent.h"
#include "Engine/Ecs/Components/MovementComponent.h"
#include "Engine/Ecs/Components/CameraComponent.h"
#include "Engine/Ecs/Components/MaterialComponent.h"
#include "Engine/Ecs/Components/MeshComponent.h"

void snd::Ecs::Init()
{
    SND_ASSERT(!Entities);

    EntityCount = 0;
    Entities = PushArray(gTranArena, Entity, kMaxEntities);
    memset(Entities, INVALID_ENTITY, kMaxEntities * sizeof(Entity));

    FreeEntityCount = 0;
    FreeEntities = PushArray(gTranArena, Entity, kMaxEntities);
    memset(FreeEntities, INVALID_ENTITY, kMaxEntities * sizeof(Entity));

    ComponentSizes[COMPONENT_TRANSFORM] = sizeof(TransformComponent);
    ComponentSizes[COMPONENT_MOVEMENT] = sizeof(MovementComponent);
    ComponentSizes[COMPONENT_CAMERA] = sizeof(CameraComponent);
    ComponentSizes[COMPONENT_MATERIAL] = sizeof(MaterialComponent);
    ComponentSizes[COMPONENT_MESH] = sizeof(MeshComponent);

    for (u16 i = 0; i < COMPONENT_COUNT; ++i)
    {
        Components[i] = PushSize(gTranArena, kMaxEntities * ComponentSizes[i]);
    }
}

snd::Entity snd::Ecs::NewEntity()
{
    if (FreeEntityCount > 0)
    {
        const u16 idx = FreeEntities[--FreeEntityCount];
        Entities[idx] = idx;
        FreeEntities[idx] = INVALID_ENTITY;
        return Entities[idx];
    }

    const u16 idx = EntityCount++;
    SND_ASSERT(idx < kMaxEntities);
    Entities[idx] = idx;
    return Entities[idx];
}

void snd::Ecs::DeleteEntity(Entity e)
{
    SND_ASSERT(e != INVALID_ENTITY);
    SND_ASSERT(e < kMaxEntities);

    if (Entities[e] == INVALID_ENTITY)
    {
        SND_WARNING("Attempt to delete invalid entity (%d)", e);
        return;
    }

    Entities[e] = INVALID_ENTITY;
    const u16 idx = FreeEntityCount++;
    SND_ASSERT(idx < kMaxEntities);
    FreeEntities[idx] = e;

    for (u16 i = 0; i < COMPONENT_COUNT; ++i)
    {
        ClearComponentData(e, (ComponentType)i);
    }
}

void* snd::Ecs::ComponentData(Entity e, ComponentType ct)
{
    SND_ASSERT(e != INVALID_ENTITY);
    SND_ASSERT(e < kMaxEntities);
    SND_ASSERT(ct != INVALID_COMPONENT_TYPE);

    return (u8*)Components[ct] + (e * ComponentSizes[ct]);
}

void snd::Ecs::ClearComponentData(Entity e, ComponentType ct)
{
    SND_ASSERT(e != INVALID_ENTITY);
    SND_ASSERT(e < kMaxEntities);
    SND_ASSERT(ct != INVALID_COMPONENT_TYPE);

    const u16 size = ComponentSizes[ct];
    memset((u8*)Components[ct] + (e * size), 0, size);
}
