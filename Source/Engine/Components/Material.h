#pragma once

#include <bgfx/bgfx.h>

struct Material
{
    bgfx::TextureHandle texture;
    bgfx::UniformHandle uniform;
};
