#pragma once

#include <bgfx/bgfx.h>

namespace snd
{
    struct MaterialComponent
    {
        bgfx::TextureHandle     Texture;
        bgfx::UniformHandle     Uniform;
    };
}
