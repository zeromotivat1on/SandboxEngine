#include "pch.h"
#include "Engine/Ecs/Ecs.h"
#include "Engine/Ecs/Components/TransformComponent.h"
#include "Engine/Ecs/Components/MovementComponent.h"
#include "Engine/Ecs/Components/CameraComponent.h"
#include "Engine/Ecs/Components/MaterialComponent.h"
#include "Engine/Ecs/Components/MeshComponent.h"
#include "Engine/Render/Vertex.h"

void ecs_init(Ecs* ecs, u32 max_entities)
{
    ecs->max_entity_count = max_entities;
    
    ecs->entity_count = 0;
    ecs->entities = arena_push_array(&g_arena_transient, max_entities, Entity);
    memset(ecs->entities, INVALID_ENTITY, max_entities * sizeof(Entity));

    ecs->free_entity_count = 0;
    ecs->free_entities = arena_push_array(&g_arena_transient, max_entities, Entity);
    memset(ecs->free_entities, INVALID_ENTITY, max_entities * sizeof(Entity));
    
    ecs->component_sizes[CT_TRANSFORM] = sizeof(TransformComponent);
    ecs->component_sizes[CT_MOVEMENT] = sizeof(MovementComponent);
    ecs->component_sizes[CT_CAMERA] = sizeof(CameraComponent);
    ecs->component_sizes[CT_MATERIAL] = sizeof(MaterialComponent);
    ecs->component_sizes[CT_MESH] = sizeof(MeshComponent);

    for (u32 i = 0; i < CT_COUNT; ++i)
    {
        sparse_set_init(&ecs->component_sets[i], &g_arena_transient, max_entities, max_entities, ecs->component_sizes[i]);
    }
}

Entity ecs_entity_new(Ecs* ecs)
{
    if (ecs->free_entity_count > 0)
    {
        const u32 last_free_idx = --ecs->free_entity_count;
        const u32 idx = ecs->free_entities[last_free_idx];
        ecs->entities[idx] = idx;
        ecs->free_entities[idx] = INVALID_ENTITY;
        return ecs->entities[idx];
    }

    const u32 idx = ecs->entity_count++;
    ASSERT(idx < ecs->max_entity_count);
    ecs->entities[idx] = idx;
    return ecs->entities[idx];
}

void ecs_entity_del(Ecs* ecs, Entity e)
{
    ASSERT(e != INVALID_ENTITY);
    ASSERT(e < ecs->max_entity_count);

    if (ecs->entities[e] == INVALID_ENTITY)
    {
        msg_warning("Attempt to delete invalid entity (%d)", e);
        return;
    }

    ecs->entities[e] = INVALID_ENTITY;
    const u32 idx = ecs->free_entity_count++;
    ASSERT(idx < ecs->max_entity_count);
    ecs->free_entities[idx] = e;

    for (u32 i = 0; i < CT_COUNT; ++i)
    {
        ecs_component_del(ecs, e, (ComponentType)i);
    }
}

void ecs_entity_iterate(Ecs* ecs, const ComponentType* cts, u8 cts_count, ecs_entity_iterate_callback callback)
{
    ASSERT(cts_count > 0);

    u64 smallest_set_index = 0;
    u64 smallest_dense_count = ecs->component_sets[cts[0]].dense_count;

    for (u64 i = 1; i < cts_count; ++i)
    {
        const u64 dense_count = ecs->component_sets[cts[i]].dense_count;
        if (dense_count < smallest_dense_count)
        {
            smallest_set_index = i;
            smallest_dense_count = dense_count;
        }
    }

    const SparseSet* smallest_set = &ecs->component_sets[cts[smallest_set_index]];
    for (u64 i = 0; i < smallest_set->dense_count; ++i)
    {
        const Entity e = smallest_set->dense_indices[i];
        bool has_all_components = true;

        for (u64 j = 0; j < cts_count; ++j)
        {
            if (j == smallest_set_index)
                continue;

            const SparseSet* other_set = &ecs->component_sets[cts[j]];
            if (!sparse_set_has(other_set, e))
            {
                has_all_components = false;
                break;
            }
        }

        if (has_all_components)
        {
            callback(ecs, e);
        }
    }
}

bool ecs_component_add(Ecs* ecs, Entity e, ComponentType ct)
{
    ASSERT(e != INVALID_ENTITY);
    ASSERT(e < ecs->max_entity_count);
    ASSERT(ct < CT_COUNT);
    
    return sparse_set_insert_zero(&ecs->component_sets[ct], e);
}

void* ecs_component_get(Ecs* ecs, Entity e, ComponentType ct)
{
    ASSERT(e != INVALID_ENTITY);
    ASSERT(e < ecs->max_entity_count);
    ASSERT(ct < CT_COUNT);

    return sparse_set_get(&ecs->component_sets[ct], e);
}

bool ecs_component_del(Ecs* ecs, Entity e, ComponentType ct)
{
    ASSERT(e != INVALID_ENTITY);
    ASSERT(e < ecs->max_entity_count);
    ASSERT(ct < CT_COUNT);

    return sparse_set_remove(&ecs->component_sets[ct], e);
}

Entity ecs_entity_new_debug_cube(Ecs* ecs)
{
    static const Vertex cube_vertices[8] =
    {
	    { vec3(-1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(0.0f), 0xff000000 },
	    { vec3( 1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(0.0f), 0xff0000ff },
	    { vec3(-1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(0.0f), 0xff00ff00 },
	    { vec3( 1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(0.0f), 0xff00ffff },
	    { vec3(-1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(0.0f), 0xffff0000 },
	    { vec3( 1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(0.0f), 0xffff00ff },
	    { vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(0.0f), 0xffffff00 },
	    { vec3( 1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(0.0f), 0xffffffff },
    };

    static const u16 cube_indices[36] =
    {
	    0, 1, 2,
	    1, 3, 2,
	    4, 6, 5,
	    5, 6, 7,
	    0, 2, 4,
	    4, 2, 6,
	    1, 5, 3,
	    5, 7, 3,
	    0, 4, 1,
	    4, 5, 1,
	    2, 3, 6,
	    6, 3, 7,
    };

    static const auto cube_vbh = bgfx::createVertexBuffer(bgfx::makeRef(cube_vertices, 8 * sizeof(Vertex)), Vertex::layout);
    static const auto cube_ibh = bgfx::createIndexBuffer(bgfx::makeRef(cube_indices, 36 * sizeof(u32)));
    static const auto cube_rph = file_program_load("base.vs.bin", "base.fs.bin");

    const Entity cube = ecs_entity_new(ecs);
    ecs_component_add(ecs, cube, CT_TRANSFORM);
    ecs_component_add(ecs, cube, CT_MESH);
    
    new (ecs_component_get(ecs, cube, CT_TRANSFORM)) TransformComponent(transform_identity());

    auto* mesh = (MeshComponent*)ecs_component_get(ecs, cube, CT_MESH);
    mesh->vbh = cube_vbh;
    mesh->ibh = cube_ibh;
    mesh->rph = cube_rph;

    return cube;
}
