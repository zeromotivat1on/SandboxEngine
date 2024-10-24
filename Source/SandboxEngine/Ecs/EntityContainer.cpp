#include "sndpch.h"
#include "SandboxEngine/Ecs/EntityContainer.h"

snd::Entity snd::EntityContainer::New()
{
    if (!m_FreeEntityIndices.empty())
    {
        const EntityIndex newIndex = m_FreeEntityIndices.back();
        m_FreeEntityIndices.pop_back();

        const Entity newID = GetEntity(newIndex, GetEntityVersion(m_Entities[newIndex]));
        m_Entities[newIndex] = newID;

        return m_Entities[newIndex];
    }

    const Entity newId = GetEntity(static_cast<EntityIndex>(m_Entities.size()), 0);
    m_Entities.emplace_back(newId);

    return m_Entities.back();
}

void snd::EntityContainer::Destroy(Entity id)
{
    // Create new entity id with invalid index and new version.
    const Entity invalidId = GetEntity(INVALID_ENTITY_INDEX, GetEntityVersion(id) + 1);

    m_Entities[GetEntityIndex(id)] = invalidId;

    m_FreeEntityIndices.push_back(GetEntityIndex(id));
}

void* snd::EntityContainer::Assign(Entity id, u16 componentId, u16 componentSize)
{
    const EntityIndex index = GetEntityIndex(id);
    if (m_Entities[index] != id)
    {
        return nullptr;
    }

    if (m_ComponentBuffers.size() <= componentId)
    {
        m_ComponentBuffers.resize(componentId + 1);
    }

    if (!m_ComponentBuffers[componentId].Valid())
    {
        m_ComponentBuffers[componentId] = SparseBuffer(gMaxEntities, componentSize);
    }

    return m_ComponentBuffers[componentId].GetOrAllocate(index);
}

void* snd::EntityContainer::Get(Entity id, u16 componentId) const
{
    const EntityIndex index = GetEntityIndex(id);
    if (m_Entities[index] != id)
    {
        return nullptr;
    }

    return m_ComponentBuffers[componentId].Get(index);
}

bool snd::EntityContainer::Remove(Entity id, u16 componentId)
{
    const EntityIndex index = GetEntityIndex(id);
    if (m_Entities[index] != id)
    {
        return false;
    }

    if (m_ComponentBuffers.size() <= componentId)
    {
        return false;
    }

    return m_ComponentBuffers[componentId].Remove(index);
}

bool snd::EntityContainer::Has(Entity id, const u16* componentIds, u8 idCount) const
{
    const EntityIndex index = GetEntityIndex(id);

    if (m_Entities[index] != id)
    {
        return false;
    }

    return Has(index, componentIds, idCount);
}

bool snd::EntityContainer::Has(EntityIndex index, const u16* componentIds, u8 idCount) const
{
    const Entity entityId = m_Entities[index];

    for (size_t i = 0; i < idCount; ++i)
    {
        if (!Get(entityId, componentIds[i]))
        {
            return false;
        }
    }

    return true;
}
