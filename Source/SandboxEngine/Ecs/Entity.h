#pragma once

#define INVALID_ENTITY_INDEX    (snd::EntityIndex)INVALID_UINDEX
#define INVALID_ENTITY_ID       snd::GetEntityId(INVALID_ENTITY_INDEX, 0)

namespace snd
{
    using                           Entity              = u64;
    using                           EntityIndex         = u32;
    using                           EntityVersion       = u32;

    inline constexpr u16            gMaxEntities       = 2048;     // max amount of entities we can process
    inline constexpr u16            gMaxComponents     = 128;      // max amount of components we can store

    inline u16 gComponentTypeCounter = 0;

    template<typename TComponent>
    u16 GetComponentId()
    {
        static u16 s_ComponentId = gComponentTypeCounter++;
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
        return ((Entity)index << 32) | (Entity)version;
    }

    SND_INLINE EntityIndex GetEntityIndex(Entity id)
    {
        // Shift down 32 so we lose the version and get our index.
        return id >> 32;
    }

    SND_INLINE EntityVersion GetEntityVersion(Entity id)
    {
        // Cast to a 32 bit int to get our version number (ignoring the top 32 bits).
        return (EntityVersion)id;
    }

    SND_INLINE bool IsEntityValid(Entity id)
    {
        // Check if the index is invalid index.
        return GetEntityIndex(id) != INVALID_ENTITY_INDEX;
    }
}
