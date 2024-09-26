#pragma once

#include <bgfx/bgfx.h>

namespace snd
{
    struct MeshComponent
    {
        bgfx::VertexBufferHandle    Vbh;    // vertex buffer handle
        bgfx::IndexBufferHandle     Ibh;    // index buffer handle
        bgfx::ProgramHandle         Rph;    // render pipeline handle
    };

    static_assert(std::is_pod_v<MeshComponent>);
}
