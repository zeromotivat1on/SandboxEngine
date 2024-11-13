#pragma once

#include <bgfx/bgfx.h>

struct MeshComponent
{
    bgfx::VertexBufferHandle    vbh;    // vertex buffer handle
    bgfx::IndexBufferHandle     ibh;    // index buffer handle
    bgfx::ProgramHandle         rph;    // render pipeline handle
};
