#pragma once

#include "SandboxEngine/Ecs/Entity.h"
#include "SandboxEngine/Ecs/EntityContainer.h"

namespace snd::ecs
{
    SND_INLINE void Init()
    {
        SND_ASSERT(!g_EntityContainer);
        void* data = memory::g_CoreStack.Push(sizeof(EntityContainer));
        g_EntityContainer = new (data) EntityContainer();
    }

    SND_INLINE void Shutdown()
    {
        SND_ASSERT(g_EntityContainer);
        g_EntityContainer = nullptr;
    }

    SND_INLINE Entity NewEntity()
    {
        return g_EntityContainer->New();
    }

    // Assign component to given entity, return default constructed component.
    template <typename TComponent>
    TComponent* Assign(Entity id)
    {
        const u16 componentId   = GetComponentId<TComponent>();
        const u16 componentSize = sizeof(TComponent);

        if (void* componentData = g_EntityContainer->Assign(id, componentId, componentSize))
        {
            TComponent* component = new(componentData) TComponent();
            return component;
        }

        return nullptr;
    }

    // Assign given component to given entity, return constructed component from given one.
    template <typename TComponent>
    TComponent* Assign(Entity id, const TComponent& component)
    {
        const u16 componentId   = GetComponentId<TComponent>();
        const u16 componentSize = sizeof(TComponent);

        if (void* componentData = g_EntityContainer->Assign(id, componentId, componentSize))
        {
            TComponent* newComponent = new(componentData) TComponent(component);
            return newComponent;
        }

        return nullptr;
    }

    template <typename TComponent>
    TComponent* Get(Entity id)
    {
        return static_cast<TComponent*>(g_EntityContainer->Get(id, GetComponentId<TComponent>()));
    }

    template <typename TComponent>
    bool Remove(Entity id)
    {
        return g_EntityContainer->Remove(id, GetComponentId<TComponent>());
    }
}
