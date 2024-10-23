#pragma once

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

            Entity                  operator*() const;
            bool                    operator==(const Iterator& other) const;
            bool                    operator!=(const Iterator& other) const;
            Iterator&               operator++();

        private:
            EntityContainer*        mContainer;
            u16*                    mIds;
            u8                      mIdCount;
            EntityIndex             mIndex;
        };

    public:
        // Max amount of unique components to filter entities from.
        static constexpr u8 sMaxComponentIdCount = 32;

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
        EntityContainer*            mContainer;    // container to grab entities and components from
        u16*                        mIds;          // component ids to observe
        u8                          mIdCount;      // amount of component ids
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
        EntityFilter        mView;                                         // filter general implementation
        u16                 mIds[EntityFilter::sMaxComponentIdCount];     // component ids to outlive filter member
    };

    // Check whether a given entity index is valid for further iteration.
    SND_INLINE bool ValidIndex(const EntityContainer* container, EntityIndex index, const u16* componentIds, u8 idCount)
    {
        return index != INVALID_ENTITY_INDEX && container->Has(index, componentIds, idCount);
    }

    // EntityFilter

    SND_INLINE EntityFilter::EntityFilter()
        : mContainer(nullptr), mIds(nullptr), mIdCount(0)
    {
    }

    SND_INLINE EntityFilter::EntityFilter(EntityContainer* container)
        : mContainer(container), mIds(nullptr), mIdCount(0)
    {
    }

    SND_INLINE EntityFilter::EntityFilter(u16* componentIds, u8 idCount)
        : mContainer(gEntityContainer), mIds(componentIds), mIdCount(idCount)
    {
        SND_ASSERT(idCount <= sMaxComponentIdCount);
    }

    SND_INLINE EntityFilter::EntityFilter(EntityContainer* container, u16* componentIds, u8 idCount)
        : mContainer(container), mIds(componentIds), mIdCount(idCount)
    {
        SND_ASSERT(idCount <= sMaxComponentIdCount);
    }

    SND_INLINE const EntityFilter::Iterator EntityFilter::begin() const
    {
        EntityIndex firstIndex = 0;

        while (firstIndex < mContainer->Count() && !ValidIndex(mContainer, firstIndex, mIds, mIdCount))
        {
            firstIndex++;
        }

        return Iterator(mContainer, firstIndex, mIds, mIdCount);
    }


    SND_INLINE const EntityFilter::Iterator EntityFilter::end() const
    {
        return Iterator(mContainer, static_cast<EntityIndex>(mContainer->Count()), mIds, mIdCount);
    }

    // EntityFilter::Iterator

    SND_INLINE EntityFilter::Iterator::Iterator(EntityContainer* container, EntityIndex index, u16* ids, u8 idCount)
        : mContainer(container), mIds(ids), mIdCount(idCount), mIndex(index)
    {
    }

    SND_INLINE Entity EntityFilter::Iterator::operator*() const
    {
        return mContainer->Data()[mIndex];
    }

    SND_INLINE bool EntityFilter::Iterator::operator==(const Iterator& other) const
    {
        return mIndex == other.mIndex || mIndex == mContainer->Count();
    }

    SND_INLINE bool EntityFilter::Iterator::operator!=(const Iterator& other) const
    {
        return !(*this == other);
    }

    SND_INLINE EntityFilter::Iterator& EntityFilter::Iterator::operator++()
    {
        do
        {
            mIndex++;
        }
        while (mIndex < mContainer->Count() && !ValidIndex(mContainer, mIndex, mIds, mIdCount));

        return *this;
    }

    // EntityFilterTemplate

    template <typename ...TComponents>
    EntityFilterTemplate<TComponents...>::EntityFilterTemplate()
    {
        constexpr u8 componentIdCount = sizeof...(TComponents);

        if constexpr (componentIdCount > 0)
        {
            GetComponentIds<TComponents...>(mIds);
            new (&mView) EntityFilter(mIds, componentIdCount);
        }
        else
        {
            new (&mView) EntityFilter();
        }
    }

    template <typename ...TComponents>
    EntityFilterTemplate<TComponents...>::EntityFilterTemplate(EntityContainer* container)
    {
        constexpr u8 componentIdCount = sizeof...(TComponents);

        if constexpr (componentIdCount > 0)
        {
            GetComponentIds<TComponents...>(mIds);
            new (&mView) EntityFilter(container, mIds, componentIdCount);
        }
        else
        {
            new (&mView) EntityFilter(container);
        }
    }

    template <typename ...TComponents>
    const typename EntityFilterTemplate<TComponents...>::Iterator EntityFilterTemplate<TComponents...>::begin() const
    {
        return mView.begin();
    }

    template <typename ...TComponents>
    const typename EntityFilterTemplate<TComponents...>::Iterator EntityFilterTemplate<TComponents...>::end() const
    {
        return mView.end();
    }
}
