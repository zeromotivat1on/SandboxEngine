#pragma once

#define INVALID_ENTITY (Entity)INDEX_NONE

typedef u32 Entity;

inline constexpr u16 k_max_entities = 2048;

enum ComponentType : u16
{
    CT_TRANSFORM = 0,
    CT_MOVEMENT,
    CT_CAMERA,
    CT_MATERIAL,
    CT_MESH,

    CT_COUNT
};

struct Ecs
{
    Entity* entities;
    Entity* free_entities;

    void*   components[CT_COUNT];
    u16     component_sizes[CT_COUNT];

    u16     max_entity_count;
    u16     entity_count;
    u16     free_entity_count;
};

void    ecs_init(Ecs* ecs, u32 max_entities);
Entity  ecs_entity_new(Ecs* ecs);
void    ecs_entity_del(Ecs* ecs, Entity e);
void*   ecs_component(Ecs* ecs, Entity e, ComponentType ct);
void    ecs_component_del(Ecs* ecs, Entity e, ComponentType ct);

// Create debug cube entity with default transform.
Entity ecs_entity_new_debug_cube(Ecs* ecs);
