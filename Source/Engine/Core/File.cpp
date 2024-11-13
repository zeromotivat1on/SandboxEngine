#include "pch.h"
#include "Engine/Core/File.h"

void path_init()
{
    PATH_ROOT = ROOT_FOLDER_PATH;
    PATH_ASSETS = ASSET_FOLDER_PATH;
    PATH_SHADERS = SHADER_FOLDER_PATH;
    PATH_SHADERS_BIN = SHADER_BINARY_FOLDER_PATH;
}

void path_make(char* out_path, u8 part_count, ...)
{
    va_list args;
    va_start(args, part_count);

    out_path[0] = '\0';

    for (s32 i = 0; i < part_count - 1; ++i)
    {
        const char* part = va_arg(args, const char*);
        strcat(out_path, part);
        strcat(out_path, "/");
    }

    const char* part = va_arg(args, const char*);
    strcat(out_path, part);

    va_end(args);
}

bgfx::ShaderHandle file_shader_load(const char* name)
{
    char shader_path[512];
    path_make(shader_path, 2, PATH_SHADERS_BIN, name);

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
