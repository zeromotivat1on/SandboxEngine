#pragma once

#include "SandboxEngine/Ecs/Entity.h"
#include "SandboxEngine/Memory/SparseBuffer.h"

namespace snd
{
    class EntityContainer
    {
    public:
        EntityId                        New();
        void                            Destroy(EntityId id);

        EntityId*                       Data();
        const EntityId*                 Data() const;
        size_t                          Count() const;
        
        void*                           Assign(EntityId id, u16 componentId, u16 componentSize);
        void*                           Get(EntityId id, u16 componentId) const;
        bool                            Remove(EntityId id, u16 componentId);
        bool                            Has(EntityId id, const u16* componentIds, u8 idCount) const;
        bool                            Has(EntityIndex index, const u16* componentIds, u8 idCount) const;

    private:
        std::vector<EntityId>           m_Entities;
        std::vector<EntityIndex>        m_FreeEntityIndices;
        std::vector<SparseBuffer>       m_ComponentBuffers;
    };

    SND_INLINE EntityId* EntityContainer::Data()
    {
        return m_Entities.data();
    }

    SND_INLINE const EntityId* EntityContainer::Data() const
    {
        return m_Entities.data();
    }

    SND_INLINE size_t EntityContainer::Count() const
    {
        return m_Entities.size();
    }
    
    // General-purpose global entity container.
    inline EntityContainer g_EntityContainer;

    namespace ecs
    {
        // Convenient functions and templates to work with global entity container.

        SND_INLINE EntityId NewEntity()
        {
            return g_EntityContainer.New();
        }
        
        template <typename TComponent>
        TComponent* Assign(EntityId id)
        {
            const u8  componentId   = GetComponentId<TComponent>();
            const u16 componentSize = sizeof(TComponent);
            
            if (void* componentData = g_EntityContainer.Assign(id, componentId, componentSize))
            {
                TComponent* component = new(componentData) TComponent();
                return component;
            }

            return nullptr;
        }

        template <typename TComponent>
        TComponent* Get(EntityId id)
        {
            return static_cast<TComponent*>(g_EntityContainer.Get(id, GetComponentId<TComponent>()));
        }

        template <typename TComponent>
        bool Remove(EntityId id)
        {
            return g_EntityContainer.Remove(id, GetComponentId<TComponent>());
        }
    }
};
