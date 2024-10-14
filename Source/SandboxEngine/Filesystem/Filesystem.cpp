#include "sndpch.h"
#include "SandboxEngine/Filesystem/Filesystem.h"
#include "SandboxEngine/Filesystem/AssetRegistry.h"

namespace snd::filesystem
{
    void Init()
    {
        SND_ASSERT(!g_AssetRegistry);
        void* data = memory::g_CoreStack.Push(sizeof(AssetRegistry));
        g_AssetRegistry = new (data) AssetRegistry();
    }

    void Shutdown()
    {
        SND_ASSERT(g_AssetRegistry);
        g_AssetRegistry = nullptr;
    }

    void MakePath(char* outPath, u8 partCount, ...)
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

    bool Read(const char* filepath, u8* buffer, u64 size, u64& outBytesRead)
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

    bgfx::ShaderHandle ReadShader(const char* name)
    {
        char shaderPath[512];
        MakePath(shaderPath, 2, g_ShaderBinaryPath, name);

        u64 bytesRead;
        memory::StackScopeBlock shaderBlock(&memory::g_FrameStack, KB(8));
        if (Read(shaderPath, (u8*)shaderBlock.Data, shaderBlock.Size, bytesRead))
        {
            const bgfx::Memory* shaderMemory = bgfx::alloc(bytesRead);
        	memcpy(shaderMemory->data, shaderBlock.Data, bytesRead);
            const bgfx::ShaderHandle handle = bgfx::createShader(shaderMemory);
            bgfx::setName(handle, name);
            return handle;
        }

        return BGFX_INVALID_HANDLE;
    }

    bgfx::ProgramHandle ReadProgram(const char* vertex, const char* fragment)
    {
    	const bgfx::ShaderHandle vsh = ReadShader(vertex);
    	const bgfx::ShaderHandle fsh = ReadShader(fragment);
    	return bgfx::createProgram(vsh, fsh, true);
    }
}
