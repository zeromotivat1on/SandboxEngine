#pragma once

#include <bgfx/bgfx.h>

inline const char* PATH_ROOT;
inline const char* PATH_ASSETS;
inline const char* PATH_SHADERS;
inline const char* PATH_SHADERS_BIN;

void path_init();
void path_make(char* out_path, u8 part_count, ...);

bgfx::ShaderHandle	file_shader_load(const char* name);
bgfx::ProgramHandle	file_program_load(const char* vertex, const char* fragment = nullptr);

