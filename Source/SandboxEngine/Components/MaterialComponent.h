#pragma once

#include <bgfx/bgfx.h>

namespace snd
{
    struct MaterialComponent
    {
        bgfx::TextureHandle     Texture;
        bgfx::UniformHandle     Uniform;
    };

    static_assert(std::is_pod_v<MaterialComponent>);
}
