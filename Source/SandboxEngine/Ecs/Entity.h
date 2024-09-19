#pragma once

#include "SandboxEngine/Core/CoreMacros.h"

#define INVALID_ENTITY_INDEX            static_cast<::snd::EntityIndex>(INVALID_INDEX)
#define INVALID_ENTITY_ID               ::snd::GetEntityId(INVALID_ENTITY_INDEX, 0)

namespace snd
{
    using                           EntityId            = uint64_t;
    using                           EntityIndex         = uint32_t;
    using                           EntityVersion       = uint32_t;
    
    inline constexpr uint16_t       g_MaxEntities       = 2048;                             // max amount of entities we can process
    inline constexpr uint16_t       g_MaxComponents     = 128;                             // max amount of components we can store
    
    using                           ComponentMask       = std::bitset<g_MaxComponents>;     // mask to determine which components entity has

    namespace ecs
    {
        inline size_t g_ComponentTypeCounter = 0;
    }
    
    template<typename TComponent>
    size_t GetComponentId()
    {
        static size_t s_ComponentId = ecs::g_ComponentTypeCounter++;
        return s_ComponentId;
    }

    template <typename ...TComponents>
    void GetComponentIds(size_t* outIds)
    {
        size_t ids[] = { GetComponentId<TComponents>()... };
        memcpy(outIds, ids, sizeof...(TComponents) * sizeof(size_t));
    }
    
    SND_INLINE EntityId GetEntityId(EntityIndex index, EntityVersion version)
    {
        // Shift the index up 32, and put the version in the bottom
        return (static_cast<EntityId>(index) << 32) | static_cast<EntityId>(version);
    }
    
    SND_INLINE EntityIndex GetEntityIndex(EntityId id)
    {
        // Shift down 32 so we lose the version and get our index.
        return id >> 32;
    }
    
    SND_INLINE EntityVersion GetEntityVersion(EntityId id)
    {
        // Cast to a 32 bit int to get our version number (ignoring the top 32 bits).
        return static_cast<EntityVersion>(id);
    }
    
    SND_INLINE bool IsEntityValid(EntityId id)
    {
        // Check if the index is invalid index.
        return GetEntityIndex(id) != INVALID_ENTITY_INDEX;
    }
}
