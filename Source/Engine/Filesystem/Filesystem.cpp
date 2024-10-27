#include "sndpch.h"
#include "Engine/Filesystem/Filesystem.h"
#include "Engine/Filesystem/AssetRegistry.h"

void snd::InitCorePaths()
{
    gRootFolderPath = ROOT_FOLDER_PATH;
    gAssetFolderPath = ASSET_FOLDER_PATH;
    gShaderFolderPath = SHADER_FOLDER_PATH;
    gShaderBinaryFolderPath = SHADER_BINARY_FOLDER_PATH;
}

void snd::MakePath(char* outPath, u8 partCount, ...)
{
    va_list args;
    va_start(args, partCount);

    outPath[0] = '\0';

    for (s32 i = 0; i < partCount - 1; ++i)
    {
        const char* part = va_arg(args, const char*);
        strcat(outPath, part);
        strcat(outPath, "/");
    }

    const char* part = va_arg(args, const char*);
    strcat(outPath, part);

    va_end(args);
}

bool snd::ReadFile(const char* filepath, u8* buffer, u64 size, u64& outBytesRead)
{
	if (FILE* handle = fopen(filepath, "rb"))
	{
		const u64 bytesRead = fread(buffer, 1, size, handle);
		const s32 err = ferror(handle);
		fclose(handle);
		if (err == 0)
		{
			outBytesRead = bytesRead;
			return true;
		}
	}
	outBytesRead = 0;
	return false;
}

bgfx::ShaderHandle snd::ReadShader(const char* name)
{
    char shaderPath[512];
    MakePath(shaderPath, 2, gShaderBinaryFolderPath, name);

    constexpr u64 buffSize = KB(8);
    u8* buff = PushSize(gFrameArena, buffSize);
    u64 bytesRead;
    
    if (ReadFile(shaderPath, buff, buffSize, bytesRead))
    {
        const bgfx::Memory* shaderMemory = bgfx::alloc(bytesRead);
    	memcpy(shaderMemory->data, buff, bytesRead);
        const bgfx::ShaderHandle handle = bgfx::createShader(shaderMemory);
        bgfx::setName(handle, name);
        gFrameArena.Pop(buffSize);
        return handle;
    }

    gFrameArena.Pop(buffSize);
    return BGFX_INVALID_HANDLE;
}

bgfx::ProgramHandle snd::ReadProgram(const char* vertex, const char* fragment)
{
	const bgfx::ShaderHandle vsh = ReadShader(vertex);
	const bgfx::ShaderHandle fsh = ReadShader(fragment);
	return bgfx::createProgram(vsh, fsh, true);
}
