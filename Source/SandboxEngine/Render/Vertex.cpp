#include "sndpch.h"
#include "SandboxEngine/Render/Vertex.h"

bgfx::VertexLayout snd::Vertex::Layout;

void snd::Vertex::InitLayout()
{
    Layout
        .begin()
        .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
        .end();
}
