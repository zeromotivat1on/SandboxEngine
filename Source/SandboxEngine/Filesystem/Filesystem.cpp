#include "sndpch.h"
#include "SandboxEngine/Filesystem/Filesystem.h"
#include "SandboxEngine/Filesystem/AssetRegistry.h"

void snd::filesystem::Init()
{
    SND_ASSERT(!g_AssetRegistry);
    g_AssetRegistry = new AssetRegistry();
}

void snd::filesystem::Shutdown()
{
    SND_ASSERT(g_AssetRegistry);
    delete g_AssetRegistry;
    g_AssetRegistry = nullptr;
}

void snd::filesystem::MakePath(char* outPath, u8 partCount, ...)
{
    va_list args;
    va_start(args, partCount);

    outPath[0] = '\0';

    for (i32 i = 0; i < partCount - 1; ++i)
    {
        const char* part = va_arg(args, const char*);
        strcat(outPath, part);
        strcat(outPath, "/");
    }

    const char* part = va_arg(args, const char*);
    strcat(outPath, part);

    va_end(args);
}

snd::Memory snd::filesystem::Read(const char* filepath)
{
    Memory mem   = { nullptr, 0 };
    FILE* handle = fopen(filepath, "rb");

    if (!handle)
    {
        SND_ERROR("Failed to open file '{}'", filepath);
        return mem;
    }

    if (fseek(handle, 0, SEEK_END) != 0)
    {
        SND_ERROR("Failed to seek through file '{}'", filepath);
        fclose(handle);
        return mem;
    }

    mem.Size = ftell(handle);

    if (mem.Size == -1)
    {
        SND_ERROR("Failed to get file size for '{}'", filepath);
        fclose(handle);
        return mem;
    }

    fseek(handle, 0, SEEK_SET);

    mem.Data = static_cast<u8*>(malloc(mem.Size));

    if (!mem.Data)
    {
        SND_ERROR("Failed to allocate buffer for file");
        fclose(handle);
        memset(&mem, 0, sizeof(mem));
        return mem;
    }

    const u64 numRead = fread(mem.Data, 1, mem.Size, handle);
    const i32 err     = ferror(handle);
    fclose(handle);

    if (err != 0)
    {
        SND_ERROR("Failed to read from file '{}'", filepath);
    }

    return mem;
}

bool snd::filesystem::Read(const char* filepath, Memory& mem)
{
    if (!Valid(mem))
    {
        SND_ERROR("Given memory is not valid");
        return false;
    }

    FILE* handle = fopen(filepath, "rb");

    if (!handle)
    {
        SND_ERROR("Failed to open file '{}'", filepath);
        return false;
    }

    const u64 numRead = fread(mem.Data, 1, mem.Size, handle);
    const i32 err     = ferror(handle);
    fclose(handle);

    if (err == 0)
    {
        return true;
    }

    SND_ERROR("Failed to read from file '{}'", filepath);
    return false;
}

bgfx::ShaderHandle snd::filesystem::ReadShader(const char* name)
{
    char shaderPath[512];
    MakePath(shaderPath, 2, g_ShaderBinaryPath, name);

    if (const Memory& mem = Read(shaderPath); Valid(mem))
    {
    	const bgfx::Memory* shaderMem   = bgfx::makeRef(mem.Data, mem.Size, MemoryRelease);
        const bgfx::ShaderHandle handle = bgfx::createShader(shaderMem);
        bgfx::setName(handle, name);
        return handle;
    }

    return BGFX_INVALID_HANDLE;
}

bgfx::ProgramHandle snd::filesystem::ReadProgram(const char* vertex, const char* fragment)
{
	const bgfx::ShaderHandle vsh = ReadShader(vertex);
	const bgfx::ShaderHandle fsh = ReadShader(fragment);
	return bgfx::createProgram(vsh, fsh, true);
}

