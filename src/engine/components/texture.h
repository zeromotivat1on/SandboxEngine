#pragma once

#include <bgfx/bgfx.h>

struct Texture
{
    bgfx::TextureHandle handle;
    bgfx::UniformHandle uniform;
};
