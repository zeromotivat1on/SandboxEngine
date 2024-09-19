#pragma once

#include "SandboxEngine/Ecs/Entity.h"
#include "SandboxEngine/Ecs/EntityContainer.h"

namespace snd
{
    class ComponentView
    {
    public:
        struct Iterator
        {
        public:
                                    Iterator(EntityContainer* container, EntityIndex index, size_t* ids, size_t idCount);
            
            EntityId                operator*() const;
            bool                    operator==(const Iterator& other) const;
            bool                    operator!=(const Iterator& other) const;
            Iterator&               operator++();

        private:
            EntityContainer*        m_Container;
            size_t*                 m_Ids;
            size_t                  m_IdCount;
            EntityIndex             m_Index;
        };
        
    public:
                                    ComponentView() = default;
                                    ComponentView(EntityContainer* container, size_t* componentIds, size_t idCount);
                                    NOT_COPYABLE(ComponentView);
                                    NOT_MOVABLE (ComponentView);
                                    ~ComponentView() = default;
        
        const Iterator              begin() const;
        const Iterator              end() const;
        
    private:
        EntityContainer*            m_Container;    // container to grab entities and components from
        size_t*                     m_Ids;          // component ids to observe
        size_t                      m_IdCount;      // amount of component ids
    };

    template <typename ...TComponents>
    class ComponentViewTemplate
    {
    public:
        using Iterator = ComponentView::Iterator;

        static constexpr uint8_t s_MaxComponentIdCount = 32;
        
    public:
                            ComponentViewTemplate(EntityContainer* container);
        
        const Iterator      begin() const;
        const Iterator      end() const;
        
    private:
        ComponentView       m_View;                         // view general implementation
        size_t              m_Ids[s_MaxComponentIdCount];   // allocated component ids to outlive component view
    };

    // Check whether a given entity index is valid for further iteration.
    SND_INLINE bool ValidIndex(const EntityContainer* container, EntityIndex index, size_t* componentIds, size_t idCount)
    {
        return index != INVALID_ENTITY_INDEX && container->Has(index, componentIds, idCount);
    }
    
    // ComponentView
    
    SND_INLINE ComponentView::ComponentView(EntityContainer* container, size_t* componentIds, size_t idCount)
        : m_Container(container), m_Ids(componentIds), m_IdCount(idCount)
    {
        
    }

    SND_INLINE const ComponentView::Iterator ComponentView::begin() const
    {
        size_t firstIndex = 0;
        
        while (firstIndex < m_Container->Count() && !ValidIndex(m_Container, firstIndex, m_Ids, m_IdCount)) 
        {
            firstIndex++;
        }
        
        return Iterator(m_Container, static_cast<EntityIndex>(firstIndex), m_Ids, m_IdCount);
    }

    
    SND_INLINE const ComponentView::Iterator ComponentView::end() const
    {
        return Iterator(m_Container, static_cast<EntityIndex>(m_Container->Count()), m_Ids, m_IdCount);
    }

    // ComponentView::Iterator

    SND_INLINE ComponentView::Iterator::Iterator(EntityContainer* container, EntityIndex index, size_t* ids, size_t idCount)
        : m_Container(container), m_Ids(ids), m_IdCount(idCount), m_Index(index)
    {
    }

    SND_INLINE EntityId ComponentView::Iterator::operator*() const
    {
        return m_Container->Data()[m_Index]; 
    }

    SND_INLINE bool ComponentView::Iterator::operator==(const Iterator& other) const
    {
        return m_Index == other.m_Index || m_Index == m_Container->Count();
    }

    SND_INLINE bool ComponentView::Iterator::operator!=(const Iterator& other) const
    {
        return !(*this == other);
    }

    SND_INLINE ComponentView::Iterator& ComponentView::Iterator::operator++()
    {
        do
        {
            m_Index++;
        }
        while (m_Index < m_Container->Count() && !ValidIndex(m_Container, m_Index, m_Ids, m_IdCount));
        
        return *this;
    }

    // ComponentViewTemplate
    
    template <typename ... TComponents>
    ComponentViewTemplate<TComponents...>::ComponentViewTemplate(EntityContainer* container)
    {
        constexpr size_t componentIdCount = sizeof...(TComponents);

        if constexpr (componentIdCount > 0)
        {
            GetComponentIds<TComponents...>(m_Ids);
            new (&m_View) ComponentView(container, m_Ids, componentIdCount);
        }
        else
        {
            new (&m_View) ComponentView(container, nullptr, 0);    
        }
    }

    template <typename ... TComponents>
    const typename ComponentViewTemplate<TComponents...>::Iterator ComponentViewTemplate<TComponents...>::begin() const
    {
        return m_View.begin();
    }

    template <typename ... TComponents>
    const typename ComponentViewTemplate<TComponents...>::Iterator ComponentViewTemplate<TComponents...>::end() const
    {
        return m_View.end();
    }
}
