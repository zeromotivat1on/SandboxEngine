#include "pch.h"
#include "core/file.h"

void init_path()
{
    PATH_ROOT = ROOT_FOLDER_PATH;
    PATH_ASSETS = ASSET_FOLDER_PATH;
    PATH_SHADERS = SHADER_FOLDER_PATH;
    PATH_TEXTURES = TEXTURE_FOLDER_PATH;
}

void make_path(char* out_path, u8 part_count, ...)
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

bgfx::ShaderHandle load_shader(const char* name)
{
    char shader_path[512];
    make_path(shader_path, 2, PATH_SHADERS, name);

    bgfx::ShaderHandle handle = BGFX_INVALID_HANDLE;
    constexpr u64 buffer_size = KB(8);
    u8* buff = arena_push_size(&g_arena_frame, buffer_size);
    u64 bytes_read = 0;
    
    if (read_file_sync(shader_path, buff, buffer_size, &bytes_read))
    {
        const bgfx::Memory* shader_memory = bgfx::alloc((u32)bytes_read);
    	memcpy(shader_memory->data, buff, bytes_read);
        handle = bgfx::createShader(shader_memory);
        bgfx::setName(handle, name);
    }

    arena_pop(&g_arena_frame, buffer_size);
    return handle;
}

bgfx::ProgramHandle load_program(const char* vertex, const char* fragment)
{
	const bgfx::ShaderHandle vsh = load_shader(vertex);
	const bgfx::ShaderHandle fsh = load_shader(fragment);
	return bgfx::createProgram(vsh, fsh, true);
}

bgfx::TextureHandle load_texture(const char* name)
{
    char texture_path[512];
    make_path(texture_path, 2, PATH_TEXTURES, name);

    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
    constexpr u64 buffer_size = MB(1);
    u8* buff = arena_push_size(&g_arena_frame, buffer_size);
    u64 bytes_read = 0;
    
    if (read_file_sync(texture_path, buff, buffer_size, &bytes_read))
    {
        const bgfx::Memory* texture_memory = bgfx::alloc((u32)bytes_read);
    	memcpy(texture_memory->data, buff, bytes_read);
        handle = bgfx::createTexture(texture_memory);
        bgfx::setName(handle, name);
    }

    arena_pop(&g_arena_frame, buffer_size);
    return handle;
}
