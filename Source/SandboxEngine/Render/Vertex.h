#pragma once

#include <bgfx/bgfx.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace snd
{
    struct Vertex
    {
        glm::vec3                   Position;
        glm::vec3                   Normal;
        glm::vec2                   Texture;
        u32                         Color;

        static bgfx::VertexLayout   Layout;
        static void                 InitLayout();
    };
}
