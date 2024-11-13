#include "pch.h"
#include "Engine/Ecs/Ecs.h"
#include "Engine/Ecs/Components/TransformComponent.h"
#include "Engine/Ecs/Components/MovementComponent.h"
#include "Engine/Ecs/Components/CameraComponent.h"
#include "Engine/Ecs/Components/MaterialComponent.h"
#include "Engine/Ecs/Components/MeshComponent.h"
#include "Engine/Render/Vertex.h"
#include "Engine/Filesystem/Filesystem.h"

void ecs_init(Ecs* ecs, u32 max_entities)
{
    ecs->max_entity_count = max_entities;
    
    ecs->entity_count = 0;
    ecs->entities = arena_push_array(&g_arena_transient, Entity, max_entities);
    memset(ecs->entities, INVALID_ENTITY, max_entities * sizeof(Entity));

    ecs->free_entity_count = 0;
    ecs->free_entities = arena_push_array(&g_arena_transient, Entity, max_entities);
    memset(ecs->free_entities, INVALID_ENTITY, max_entities * sizeof(Entity));
    
    ecs->component_sizes[CT_TRANSFORM] = sizeof(TransformComponent);
    ecs->component_sizes[CT_MOVEMENT] = sizeof(MovementComponent);
    ecs->component_sizes[CT_CAMERA] = sizeof(CameraComponent);
    ecs->component_sizes[CT_MATERIAL] = sizeof(MaterialComponent);
    ecs->component_sizes[CT_MESH] = sizeof(MeshComponent);

    for (u16 i = 0; i < CT_COUNT; ++i)
    {
        ecs->components[i] = arena_push_zero(&g_arena_transient, max_entities * ecs->component_sizes[i]);
    }
}

Entity ecs_entity_new(Ecs* ecs)
{
    if (ecs->free_entity_count > 0)
    {
        const u16 idx = ecs->free_entities[ecs->free_entity_count - 1];
        ecs->entities[idx] = idx;
        ecs->free_entities[idx] = INVALID_ENTITY;
        ecs->free_entity_count--;
        return ecs->entities[idx];
    }

    const u16 idx = ecs->entity_count++;
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
    const u16 idx = ecs->free_entity_count++;
    ASSERT(idx < ecs->max_entity_count);
    ecs->free_entities[idx] = e;

    for (u16 i = 0; i < CT_COUNT; ++i)
    {
        ecs_component_del(ecs, e, (ComponentType)i);
    }
}

void* ecs_component(Ecs* ecs, Entity e, ComponentType ct)
{
    ASSERT(e != INVALID_ENTITY);
    ASSERT(e < ecs->max_entity_count);
    ASSERT(ct >= 0 && ct < CT_COUNT);

    return (u8*)ecs->components[ct] + (e * ecs->component_sizes[ct]);
}

void ecs_component_del(Ecs* ecs, Entity e, ComponentType ct)
{
    ASSERT(e != INVALID_ENTITY);
    ASSERT(e < ecs->max_entity_count);
    ASSERT(ct >= 0 && ct < CT_COUNT);

    const u16 ctsize = ecs->component_sizes[ct];
    memset((u8*)ecs->components[ct] + (e * ctsize), 0, ctsize);
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

    new (ecs_component(ecs, cube, CT_TRANSFORM)) TransformComponent(IdentityTransform());

    auto* mesh = (MeshComponent*)ecs_component(ecs, cube, CT_MESH);
    mesh->vbh = cube_vbh;
    mesh->ibh = cube_ibh;
    mesh->rph = cube_rph;

    return cube;
}
