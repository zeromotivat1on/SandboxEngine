#pragma once

#include <bgfx/bgfx.h>

namespace snd
{
    struct Vertex
    {
        vec3                        Position;
        vec3                        Normal;
        vec2                        Texture;
        u32                         Color;

        static bgfx::VertexLayout   Layout;
        static void                 InitLayout();
    };
}
