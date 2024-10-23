#pragma once

#include "SandboxEngine/Ecs/Entity.h"
#include "SandboxEngine/Ecs/EntityContainer.h"

namespace snd
{
    inline EntityContainer* gEntityContainer;

    SND_INLINE Entity NewEntity()
    {
        return gEntityContainer->New();
    }

    // Assign component to given entity, return default constructed component.
    template <typename TComponent>
    TComponent* AssignComponent(Entity id)
    {
        const u16 componentId   = GetComponentId<TComponent>();
        const u16 componentSize = sizeof(TComponent);

        if (void* componentData = gEntityContainer->Assign(id, componentId, componentSize))
        {
            TComponent* component = new(componentData) TComponent();
            return component;
        }

        return nullptr;
    }

    // Assign given component to given entity, return constructed component from given one.
    template <typename TComponent>
    TComponent* AssignComponent(Entity id, const TComponent& component)
    {
        const u16 componentId   = GetComponentId<TComponent>();
        const u16 componentSize = sizeof(TComponent);

        if (void* componentData = gEntityContainer->Assign(id, componentId, componentSize))
        {
            TComponent* newComponent = new(componentData) TComponent(component);
            return newComponent;
        }

        return nullptr;
    }

    template <typename TComponent>
    TComponent* GetComponent(Entity id)
    {
        return (TComponent*)gEntityContainer->Get(id, GetComponentId<TComponent>());
    }

    template <typename TComponent>
    bool RemoveComponent(Entity id)
    {
        return gEntityContainer->Remove(id, GetComponentId<TComponent>());
    }
}
