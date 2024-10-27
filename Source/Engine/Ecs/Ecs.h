#pragma once

#define INVALID_ENTITY          (Entity)INVALID_INDEX
#define INVALID_COMPONENT_TYPE  (ComponentType)INVALID_INDEX

namespace snd
{
    using Entity = u32;

    inline constexpr u16 kMaxEntities = 2048;

    enum ComponentType : u16
    {
        COMPONENT_TRANSFORM,
        COMPONENT_MOVEMENT,
        COMPONENT_CAMERA,
        COMPONENT_MATERIAL,
        COMPONENT_MESH,

        COMPONENT_COUNT
    };

    struct Ecs
    {
        Entity* Entities;
        Entity* FreeEntities;
        void*   Components[COMPONENT_COUNT];
        u16     ComponentSizes[COMPONENT_COUNT];
        u16     EntityCount;
        u16     FreeEntityCount;

        void    Init();
        void    Tick(f32 dt);

        Entity  NewEntity();
        void    DeleteEntity(Entity e);

        void*   ComponentData(Entity e, ComponentType ct);
        void    ClearComponentData(Entity e, ComponentType ct);
    };

    inline Ecs* gEcs;
}
