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
        u16                             Count() const;
        
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

    // General-purpose global entity container.
    inline EntityContainer* g_EntityContainer = nullptr;
    
    SND_INLINE EntityId* EntityContainer::Data()
    {
        return m_Entities.data();
    }

    SND_INLINE const EntityId* EntityContainer::Data() const
    {
        return m_Entities.data();
    }

    SND_INLINE u16 EntityContainer::Count() const
    {
        return static_cast<u16>(m_Entities.size());
    }
};
