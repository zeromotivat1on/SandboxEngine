#pragma once

#include <bgfx/bgfx.h>

namespace snd::filesystem
{
    inline const char* g_RootPath           = PROJECT_SOURCE_DIR;
    inline const char* g_ShaderPath         = SHADER_DIR;
    inline const char* g_ShaderBinaryPath   = SHADER_BINARY_DIR;

    inline class AssetRegistry* g_AssetRegistry = nullptr;

    void Init();
    void Shutdown();

	void MakePath(char* outPath, u8 partCount, ...);

    Memory  Read(const char* filepath);
    bool    Read(const char* filepath, Memory& mem);

	bgfx::ShaderHandle	ReadShader(const char* name);
	bgfx::ProgramHandle ReadProgram(const char* vertex, const char* fragment = nullptr);
}
