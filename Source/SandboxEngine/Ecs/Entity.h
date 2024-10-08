﻿#pragma once

#define INVALID_ENTITY_INDEX            static_cast<::snd::EntityIndex>(INVALID_UINDEX)
#define INVALID_ENTITY_ID               ::snd::GetEntityId(INVALID_ENTITY_INDEX, 0)

namespace snd
{
    using                           Entity              = u64;
    using                           EntityIndex         = u32;
    using                           EntityVersion       = u32;

    inline constexpr u16            g_MaxEntities       = 2048;     // max amount of entities we can process
    inline constexpr u16            g_MaxComponents     = 128;      // max amount of components we can store

    namespace ecs
    {
        // Global counter of unique components, do not modify directly.
        inline u16 g_ComponentTypeCounter = 0;
    }

    template<typename TComponent>
    u16 GetComponentId()
    {
        static u16 s_ComponentId = ecs::g_ComponentTypeCounter++;
        return s_ComponentId;
    }

    template <typename ...TComponents>
    void GetComponentIds(u16* outIds)
    {
        u16 ids[] = { GetComponentId<TComponents>()... };
        memcpy(outIds, ids, sizeof...(TComponents) * sizeof(u16));
    }

    SND_INLINE Entity GetEntity(EntityIndex index, EntityVersion version)
    {
        // Shift the index up 32, and put the version in the bottom
        return (static_cast<Entity>(index) << 32) | static_cast<Entity>(version);
    }

    SND_INLINE EntityIndex GetEntityIndex(Entity id)
    {
        // Shift down 32 so we lose the version and get our index.
        return id >> 32;
    }

    SND_INLINE EntityVersion GetEntityVersion(Entity id)
    {
        // Cast to a 32 bit int to get our version number (ignoring the top 32 bits).
        return static_cast<EntityVersion>(id);
    }

    SND_INLINE bool IsEntityValid(Entity id)
    {
        // Check if the index is invalid index.
        return GetEntityIndex(id) != INVALID_ENTITY_INDEX;
    }
}
