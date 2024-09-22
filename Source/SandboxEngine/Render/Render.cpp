#include "sndpch.h"
#include "SandboxEngine/Render/Render.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Core/Timer.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Core/FileSystem.h"
#include "SandboxEngine/World/Camera.h"
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <glm/gtc/type_ptr.hpp>

// Current window we render on.
static snd::Window* s_Window = nullptr;
static snd::Camera* s_Camera = nullptr;

struct PosColorVertex
{
	f32 m_x;
	f32 m_y;
	f32 m_z;
	u32 m_abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const u16 s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

static bgfx::ProgramHandle s_Program;
static bgfx::VertexBufferHandle s_vbh;
static bgfx::IndexBufferHandle s_ibh;

void snd::render::Init(Window* window)
{
	s_Window = window;

	bgfx::Init bgfxInit;
	bgfxInit.type = bgfx::RendererType::OpenGL;
	bgfxInit.platformData.nwh = window->NativeHandle();
	bgfxInit.resolution.width = window->Width();
	bgfxInit.resolution.height = window->Height();

	if (window->Vsync())
	{
		bgfxInit.resolution.reset |= BGFX_RESET_VSYNC;
	}

	bgfx::init(bgfxInit);

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);

#ifdef SND_BUILD_DEBUG
	bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

	SND_INFO("Using Renderer API \"{}\"", bgfx::getRendererName(bgfx::getRendererType()));

	s_Program = fs::LoadProgram("base.vs", "base.fs");

	PosColorVertex::init();

	s_vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout);
	s_ibh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
}

void snd::render::Shutdown()
{
	bgfx::shutdown();
}

void snd::render::OnWindowResized(u16 width, u16 height)
{
	u32 flags = 0;

	if (s_Window->Vsync())
	{
		flags |= BGFX_RESET_VSYNC;
	}

	bgfx::reset(width, height, flags);
}

void snd::render::SetCamera(snd::Camera* camera)
{
	SND_ASSERT(camera);
	s_Camera = camera;
}

std::string ToString(const glm::vec3& vec)
{
	std::ostringstream oss;
	oss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return oss.str();
}

std::string ToString(const glm::vec2& vec)
{
	std::ostringstream oss;
	oss << "(" << vec.x << ", " << vec.y << ")";
	return oss.str();
}

void snd::render::Tick(f32 dt)
{
	bgfx::setViewTransform(0, glm::value_ptr(s_Camera->ViewMatrix()), glm::value_ptr(s_Camera->ProjectionMatrix()));
	bgfx::setViewRect(0, 0, 0, s_Window->Width(), s_Window->Height());

	// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	bgfx::dbgTextClear();

	const f32 time = StartupTime();

	u8 dbgTextY = 1;
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Window size: %dx%d", s_Window->Width(), s_Window->Height());
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Run time: %.2fs", time);
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Delta time: %.2fms", (dt * 1000.0f));
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "FPS: %.2f", (1.0f / dt));
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Vsync: %s", s_Window->Vsync() ? "ON" : "OFF");

	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Camera: location %s, target %s", ToString(s_Camera->Location()).c_str(), ToString(s_Camera->Target()).c_str());
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Mouse: position %s", ToString(input::MousePosition()).c_str());
	
	for (u8 yy = 0; yy < 11; ++yy)
	{
		for (u8 xx = 0; xx < 11; ++xx)
		{
			f32 mtx[16];
			bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
			mtx[12] = -15.0f + static_cast<f32>(xx) * 3.0f;
			mtx[13] = -15.0f + static_cast<f32>(yy) * 3.0f;
			mtx[14] = 0.0f;

			// Set model matrix for rendering.
			bgfx::setTransform(mtx);

			// Set vertex and index buffer.
			bgfx::setVertexBuffer(0, s_vbh);
			bgfx::setIndexBuffer(s_ibh);

			// Set render states.
			bgfx::setState(BGFX_STATE_DEFAULT);

			// Submit primitive for rendering to view 0.
			bgfx::submit(0, s_Program);
		}
	}

	bgfx::frame();
}
