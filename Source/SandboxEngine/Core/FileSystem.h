#pragma once

#include <bgfx/bgfx.h>

namespace snd::fs
{
	bgfx::ShaderHandle	LoadShader(const char* name);
	bgfx::ProgramHandle LoadProgram(const char* vsName, const char* fsName = nullptr);
}
