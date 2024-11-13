#pragma once

#include <bgfx/bgfx.h>

inline const char* gRootFolderPath;
inline const char* gAssetFolderPath;
inline const char* gShaderFolderPath;
inline const char* gShaderBinaryFolderPath;

inline class AssetRegistry* gAssetRegistry;

void path_init();
void path_make(char* outPath, u8 partCount, ...);

bgfx::ShaderHandle	file_shader_load(const char* name);
bgfx::ProgramHandle	file_program_load(const char* vertex, const char* fragment = nullptr);

