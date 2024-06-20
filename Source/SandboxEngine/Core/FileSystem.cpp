#include <sndpch.h>
#include "FileSystem.h"
#include "SandboxEngine/Core/Core.h"
#include <bx/file.h>

static bx::FileReader s_FileReader = bx::FileReader();

static const bgfx::Memory* LoadMemory(bx::FileReaderI* reader, const bx::FilePath& path)
{
	if (bx::open(reader, path))
	{
		const uint32_t size = (uint32_t)bx::getSize(reader);
		const bgfx::Memory* mem = bgfx::alloc(size + 1);
		
		bx::read(reader, mem->data, size, bx::ErrorAssert());
		bx::close(reader);
		
		mem->data[mem->size - 1] = '\0';

		return mem;
	}

	SND_LOG_ERROR("Failed to load \"{}\"", path.getCPtr());
	return nullptr;
}

static bgfx::ShaderHandle LoadShader(bx::FileReaderI* reader, const char* name)
{
	bx::FilePath filePath("Shaders/Bin/");

	//switch (bgfx::getRendererType())
	//{
	//case bgfx::RendererType::Noop:
	//case bgfx::RendererType::Direct3D11:
	//case bgfx::RendererType::Direct3D12:	filePath.join("dx11");  break;
	//case bgfx::RendererType::Agc:
	//case bgfx::RendererType::Gnm:			filePath.join("pssl");  break;
	//case bgfx::RendererType::Metal:			filePath.join("metal"); break;
	//case bgfx::RendererType::Nvn:			filePath.join("nvn");   break;
	//case bgfx::RendererType::OpenGL:		filePath.join("glsl");  break;
	//case bgfx::RendererType::OpenGLES:		filePath.join("essl");  break;
	//case bgfx::RendererType::Vulkan:		filePath.join("spirv"); break;
	//case bgfx::RendererType::Count:		
	//default:								SND_ASSERT(false, "You should not be here!"); break;
	//}

	char fileName[512];
	bx::strCopy(fileName, BX_COUNTOF(fileName), name);
	bx::strCat(fileName, BX_COUNTOF(fileName), ".bin");

	filePath.join(fileName);

	const bgfx::Memory* shaderMemory = LoadMemory(reader, filePath);
	bgfx::ShaderHandle handle = bgfx::createShader(shaderMemory);
	bgfx::setName(handle, name);

	return handle;
}

bgfx::ShaderHandle snd::fs::LoadShader(const char* name)
{
	return ::LoadShader(&s_FileReader, name);
}

static bgfx::ProgramHandle LoadProgram(bx::FileReaderI* reader, const char* vsName, const char* fsName)
{
	bgfx::ShaderHandle vsh = LoadShader(reader, vsName);
	bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;

	if (fsName)
	{
		fsh = LoadShader(reader, fsName);
	}

	return bgfx::createProgram(vsh, fsh, true);
}

bgfx::ProgramHandle snd::fs::LoadProgram(const char* vsName, const char* fsName /*= nullptr*/)
{
	return ::LoadProgram(&s_FileReader, vsName, fsName);
}
