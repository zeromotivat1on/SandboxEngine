#pragma once

#include <bgfx/bgfx.h>

inline const char* PATH_ROOT;
inline const char* PATH_ASSETS;
inline const char* PATH_SHADERS;
inline const char* PATH_TEXTURES;

void init_path();
void make_path(char* out_path, u8 part_count, ...);

bgfx::ShaderHandle	load_shader(const char* name);
bgfx::ProgramHandle	load_program(const char* vertex, const char* fragment = nullptr);
bgfx::TextureHandle load_texture(const char* name);
