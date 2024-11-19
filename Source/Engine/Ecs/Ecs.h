#pragma once

#define INVALID_ENTITY (Entity)INDEX_NONE

typedef u32 Entity;
typedef void(*ecs_entity_iterate_callback)(struct Ecs* ecs, Entity e);

inline constexpr u32 k_max_entities = 100 * 1000;

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
    Entity*     entities;
    Entity*     free_entities;

    SparseSet   component_sets[CT_COUNT];
    u32         component_sizes[CT_COUNT];

    u32         max_entity_count;
    u32         entity_count;
    u32         free_entity_count;
};

// NOTE: Maybe store components in hash tables with keys as string == component type name?
// ecs_component_register(ecs, sid(component_name), component_size);
//#define ecs_component_struct(ecs, e, type) (type*)ecs_component_get(ecs, e, MACRO_STRING(type));
//CameraComponent/auto* camera = ecs_component_struct(ecs, entity, CameraComponent);

void    ecs_init(Ecs* ecs, u32 max_entities);
Entity  ecs_entity_new(Ecs* ecs);
void    ecs_entity_del(Ecs* ecs, Entity e);
void    ecs_entity_iterate(Ecs* ecs, const ComponentType* cts, u8 cts_count, ecs_entity_iterate_callback callback);
bool    ecs_component_add(Ecs* ecs, Entity e, ComponentType ct);
void*   ecs_component_get(Ecs* ecs, Entity e, ComponentType ct);
bool    ecs_component_del(Ecs* ecs, Entity e, ComponentType ct);

// Create debug cube entity with default transform.
Entity ecs_entity_new_debug_cube(Ecs* ecs);
