#pragma once

#include <bgfx/bgfx.h>

struct MaterialComponent
{
    bgfx::TextureHandle texture;
    bgfx::UniformHandle uniform;
};
