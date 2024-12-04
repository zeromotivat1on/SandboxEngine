#pragma once

#include <bgfx/bgfx.h>

struct Vertex
{
    vec3                        position;
    vec3                        normal;
    vec2                        texture;
    u32                         color;

    static bgfx::VertexLayout   layout;
    static void                 init_layout();
};
