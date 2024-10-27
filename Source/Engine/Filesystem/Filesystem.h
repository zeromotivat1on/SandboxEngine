#pragma once

#include <bgfx/bgfx.h>

namespace snd
{
    inline const char* gRootFolderPath;
    inline const char* gAssetFolderPath;
    inline const char* gShaderFolderPath;
    inline const char* gShaderBinaryFolderPath;

    inline class AssetRegistry* gAssetRegistry;

    void InitCorePaths();

	void MakePath(char* outPath, u8 partCount, ...);

    // Sync read file as binary.
    bool ReadFile(const char* filepath, u8* buffer, u64 size, u64& outBytesRead);

	bgfx::ShaderHandle	ReadShader(const char* name);
	bgfx::ProgramHandle ReadProgram(const char* vertex, const char* fragment = nullptr);
}
