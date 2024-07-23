#include "sndpch.h"
#include "SandboxEngine/Render/Render.h"
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
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

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

static const uint16_t s_cubeTriList[] =
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
static snd::Timer s_TestTimer;

void snd::render::Init(Window* window)
{
	s_Window = window;

	bgfx::Init bgfxInit;
	bgfxInit.type = bgfx::RendererType::OpenGL;
	bgfxInit.platformData.nwh = window->GetNativeHandle();
	bgfxInit.resolution.width = window->GetWidth();
	bgfxInit.resolution.height = window->GetHeight();

	if (window->IsVsync())
	{
		bgfxInit.resolution.reset |= BGFX_RESET_VSYNC;
	}

	bgfx::init(bgfxInit);

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);

#ifdef SND_DEBUG
	bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

	SND_LOG_INFO("Using Renderer API \"{}\"", bgfx::getRendererName(bgfx::getRendererType()));

	s_Program = fs::LoadProgram("base.vs", "base.fs");

	PosColorVertex::init();

	s_vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout);
	s_ibh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

	s_TestTimer.Start();
}

void snd::render::Shutdown()
{
	bgfx::shutdown();
}

void snd::render::OnWindowResized(uint32_t width, uint32_t height)
{
	uint32_t flags = 0;

	if (s_Window->IsVsync())
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

void snd::render::Tick(float dt)
{
	bgfx::setViewTransform(0, glm::value_ptr(s_Camera->GetViewMatrix()), glm::value_ptr(s_Camera->GetProjectionMatrix()));
	bgfx::setViewRect(0, 0, 0, (uint16_t)s_Window->GetWidth(), (uint16_t)s_Window->GetHeight());

	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	bgfx::dbgTextClear();

	const float time = s_TestTimer.Elapsed();

	bgfx::dbgTextPrintf(1, 1, 0x0f, "Window size: %dx%d", s_Window->GetWidth(), s_Window->GetHeight());
	bgfx::dbgTextPrintf(1, 2, 0x0f, "Run time: %.2fs", time);
	bgfx::dbgTextPrintf(1, 3, 0x0f, "Delta time: %.2fms", (dt * 1000.0f));
	bgfx::dbgTextPrintf(1, 4, 0x0f, "FPS: %.2f", (1.0f / dt));
	bgfx::dbgTextPrintf(1, 5, 0x0f, "Vsync: %s", s_Window->IsVsync() ? "ON" : "OFF");

	bgfx::dbgTextPrintf(1, 6, 0x0f, "Camera: location %s, target %s", ToString(s_Camera->GetLocation()).c_str(), ToString(s_Camera->GetTarget()).c_str());

	for (uint32_t yy = 0; yy < 11; ++yy)
	{
		for (uint32_t xx = 0; xx < 11; ++xx)
		{
			float mtx[16];
			bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
			mtx[12] = -15.0f + float(xx) * 3.0f;
			mtx[13] = -15.0f + float(yy) * 3.0f;
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
