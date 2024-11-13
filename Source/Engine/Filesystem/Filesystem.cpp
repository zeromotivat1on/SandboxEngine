#include "pch.h"
#include "Engine/Filesystem/Filesystem.h"
#include "Engine/Filesystem/AssetRegistry.h"

void path_init()
{
    gRootFolderPath = ROOT_FOLDER_PATH;
    gAssetFolderPath = ASSET_FOLDER_PATH;
    gShaderFolderPath = SHADER_FOLDER_PATH;
    gShaderBinaryFolderPath = SHADER_BINARY_FOLDER_PATH;
}

void path_make(char* outPath, u8 partCount, ...)
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

bgfx::ShaderHandle file_shader_load(const char* name)
{
    char shader_path[512];
    path_make(shader_path, 2, gShaderBinaryFolderPath, name);

    bgfx::ShaderHandle handle = BGFX_INVALID_HANDLE;
    constexpr u64 buffer_size = KB(8);
    u8* buff = arena_push_size(&g_arena_frame, buffer_size);
    u64 bytes_read = 0;
    
    if (file_read_sync(shader_path, buff, buffer_size, &bytes_read))
    {
        const bgfx::Memory* shaderMemory = bgfx::alloc((u32)bytes_read);
    	memcpy(shaderMemory->data, buff, bytes_read);
        handle = bgfx::createShader(shaderMemory);
        bgfx::setName(handle, name);
    }

    arena_pop(&g_arena_frame, buffer_size);
    return handle;
}

bgfx::ProgramHandle file_program_load(const char* vertex, const char* fragment)
{
	const bgfx::ShaderHandle vsh = file_shader_load(vertex);
	const bgfx::ShaderHandle fsh = file_shader_load(fragment);
	return bgfx::createProgram(vsh, fsh, true);
}
