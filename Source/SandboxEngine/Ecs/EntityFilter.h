#pragma once

#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Ecs/Entity.h"
#include "SandboxEngine/Ecs/EntityContainer.h"

namespace snd
{
    // Class to iterate through entities by filtering using components.
    class EntityFilter
    {
    public:
        struct Iterator
        {
        public:
                                    Iterator(EntityContainer* container, EntityIndex index, u16* ids, u8 idCount);
            
            EntityId                operator*() const;
            bool                    operator==(const Iterator& other) const;
            bool                    operator!=(const Iterator& other) const;
            Iterator&               operator++();

        private:
            EntityContainer*        m_Container;
            u16*                    m_Ids;
            u8                      m_IdCount;
            EntityIndex             m_Index;
        };

    public:
        // Max amount of unique components to filter entities from.
        static constexpr u8 s_MaxComponentIdCount = 32;

    public:
                                    EntityFilter();
                                    EntityFilter(EntityContainer* container);
                                    EntityFilter(u16* componentIds, u8 idCount);
                                    EntityFilter(EntityContainer* container, u16* componentIds, u8 idCount);
                                    NOT_COPYABLE(EntityFilter);
                                    NOT_MOVABLE (EntityFilter);
                                    ~EntityFilter() = default;
        
        const Iterator              begin() const;
        const Iterator              end() const;
        
    private:
        EntityContainer*            m_Container;    // container to grab entities and components from
        u16*                        m_Ids;          // component ids to observe
        u8                          m_IdCount;      // amount of component ids
    };

    template <typename ...TComponents>
    class EntityFilterTemplate
    {
    public:
        using Iterator      = EntityFilter::Iterator;
    
    public:
                            EntityFilterTemplate();
                            EntityFilterTemplate(EntityContainer* container);
                            NOT_COPYABLE(EntityFilterTemplate);
                            NOT_MOVABLE (EntityFilterTemplate);
                            ~EntityFilterTemplate() = default;
        
        const Iterator      begin() const;
        const Iterator      end() const;
        
    private:
        EntityFilter        m_View;                                         // filter general implementation
        u16                 m_Ids[EntityFilter::s_MaxComponentIdCount];     // component ids to outlive filter member
    };

    // Check whether a given entity index is valid for further iteration.
    SND_INLINE bool ValidIndex(const EntityContainer* container, EntityIndex index, const u16* componentIds, u8 idCount)
    {
        return index != INVALID_ENTITY_INDEX && container->Has(index, componentIds, idCount);
    }
    
    // EntityFilter

    SND_INLINE EntityFilter::EntityFilter()
        : m_Container(nullptr), m_Ids(nullptr), m_IdCount(0)
    {
    }

    SND_INLINE EntityFilter::EntityFilter(EntityContainer* container)
        : m_Container(container), m_Ids(nullptr), m_IdCount(0)
    {
    }

    SND_INLINE EntityFilter::EntityFilter(u16* componentIds, u8 idCount)
        : m_Container(&g_EntityContainer), m_Ids(componentIds), m_IdCount(idCount)
    {
        SND_ASSERT(idCount <= s_MaxComponentIdCount);
    }

    SND_INLINE EntityFilter::EntityFilter(EntityContainer* container, u16* componentIds, u8 idCount)
        : m_Container(container), m_Ids(componentIds), m_IdCount(idCount)
    {
        SND_ASSERT(idCount <= s_MaxComponentIdCount);
    }

    SND_INLINE const EntityFilter::Iterator EntityFilter::begin() const
    {
        EntityIndex firstIndex = 0;
        
        while (firstIndex < m_Container->Count() && !ValidIndex(m_Container, firstIndex, m_Ids, m_IdCount)) 
        {
            firstIndex++;
        }
        
        return Iterator(m_Container, firstIndex, m_Ids, m_IdCount);
    }

    
    SND_INLINE const EntityFilter::Iterator EntityFilter::end() const
    {
        return Iterator(m_Container, static_cast<EntityIndex>(m_Container->Count()), m_Ids, m_IdCount);
    }

    // EntityFilter::Iterator

    SND_INLINE EntityFilter::Iterator::Iterator(EntityContainer* container, EntityIndex index, u16* ids, u8 idCount)
        : m_Container(container), m_Ids(ids), m_IdCount(idCount), m_Index(index)
    {
    }

    SND_INLINE EntityId EntityFilter::Iterator::operator*() const
    {
        return m_Container->Data()[m_Index]; 
    }

    SND_INLINE bool EntityFilter::Iterator::operator==(const Iterator& other) const
    {
        return m_Index == other.m_Index || m_Index == m_Container->Count();
    }

    SND_INLINE bool EntityFilter::Iterator::operator!=(const Iterator& other) const
    {
        return !(*this == other);
    }

    SND_INLINE EntityFilter::Iterator& EntityFilter::Iterator::operator++()
    {
        do
        {
            m_Index++;
        }
        while (m_Index < m_Container->Count() && !ValidIndex(m_Container, m_Index, m_Ids, m_IdCount));
        
        return *this;
    }

    // EntityFilterTemplate

    template <typename ...TComponents>
    EntityFilterTemplate<TComponents...>::EntityFilterTemplate()
    {
        constexpr u8 componentIdCount = sizeof...(TComponents);

        if constexpr (componentIdCount > 0)
        {
            GetComponentIds<TComponents...>(m_Ids);
            new (&m_View) EntityFilter(m_Ids, componentIdCount);
        }
        else
        {
            new (&m_View) EntityFilter();    
        }
    }
    
    template <typename ...TComponents>
    EntityFilterTemplate<TComponents...>::EntityFilterTemplate(EntityContainer* container)
    {
        constexpr u8 componentIdCount = sizeof...(TComponents);

        if constexpr (componentIdCount > 0)
        {
            GetComponentIds<TComponents...>(m_Ids);
            new (&m_View) EntityFilter(container, m_Ids, componentIdCount);
        }
        else
        {
            new (&m_View) EntityFilter(container);    
        }
    }

    template <typename ...TComponents>
    const typename EntityFilterTemplate<TComponents...>::Iterator EntityFilterTemplate<TComponents...>::begin() const
    {
        return m_View.begin();
    }

    template <typename ...TComponents>
    const typename EntityFilterTemplate<TComponents...>::Iterator EntityFilterTemplate<TComponents...>::end() const
    {
        return m_View.end();
    }
}
