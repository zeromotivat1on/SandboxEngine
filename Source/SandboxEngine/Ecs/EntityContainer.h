#pragma once

#include "SandboxEngine/Ecs/Entity.h"

namespace snd
{
    class EntityContainer
    {
    public:
        Entity                          New();
        void                            Destroy(Entity id);

        Entity*                         Data();
        const Entity*                   Data() const;
        u16                             Count() const;

        void*                           Assign(Entity id, u16 componentId, u16 componentSize);
        void*                           Get(Entity id, u16 componentId) const;
        bool                            Remove(Entity id, u16 componentId);
        bool                            Has(Entity id, const u16* componentIds, u8 idCount) const;
        bool                            Has(EntityIndex index, const u16* componentIds, u8 idCount) const;

    private:
        std::vector<Entity>             m_Entities;
        std::vector<EntityIndex>        m_FreeEntityIndices;
        std::vector<SparseBuffer>       m_ComponentBuffers;
    };

    // General-purpose global entity container.
    inline EntityContainer* g_EntityContainer = nullptr;

    SND_INLINE Entity* EntityContainer::Data()
    {
        return m_Entities.data();
    }

    SND_INLINE const Entity* EntityContainer::Data() const
    {
        return m_Entities.data();
    }

    SND_INLINE u16 EntityContainer::Count() const
    {
        return static_cast<u16>(m_Entities.size());
    }
};
