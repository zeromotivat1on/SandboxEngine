#include "sndpch.h"
#include "SandboxEngine/Render/Render.h"
#include "SandboxEngine/Render/Vertex.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Core/Timer.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Core/FileSystem.h"
#include "SandboxEngine/Components/CameraComponent.h"
#include "SandboxEngine/Components/MeshComponent.h"
#include "SandboxEngine/Components/TransformComponent.h"
#include "SandboxEngine/Ecs/EntityContainer.h"
#include "SandboxEngine/Ecs/EntityFilter.h"
#include <bgfx/bgfx.h>
#include <glm/gtc/type_ptr.hpp>

// Current window we render on.
static snd::Window*					s_Window = nullptr;
static const snd::CameraComponent*	s_Camera = nullptr;

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

	Vertex::InitLayout();

	// Create debug test cubes. 
	for (i32 i = 0; i < 10; ++i)
	{
		const EntityId cube = NewEntityDebugCube();
		ecs::Get<TransformComponent>(cube)->Location.x += i * 10.0f;
	}
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

void snd::render::SetCamera(const CameraComponent* camera)
{
	SND_ASSERT(camera);
	s_Camera = camera;
}

snd::EntityId snd::render::NewEntityDebugCube()
{
	static const Vertex s_CubeVertices[8] =
	{
		{ glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f), glm::vec2(0.0f), 0xff000000 },
		{ glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(1.0f), glm::vec2(0.0f), 0xff0000ff },
		{ glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f), glm::vec2(0.0f), 0xff00ff00 },
		{ glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(1.0f), glm::vec2(0.0f), 0xff00ffff },
		{ glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(1.0f), glm::vec2(0.0f), 0xffff0000 },
		{ glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(1.0f), glm::vec2(0.0f), 0xffff00ff },
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f), glm::vec2(0.0f), 0xffffff00 },
		{ glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(1.0f), glm::vec2(0.0f), 0xffffffff },
	};

	static const u16 s_CubeIndices[36] =
	{
		0, 1, 2,
		1, 3, 2,
		4, 6, 5,
		5, 6, 7,
		0, 2, 4,
		4, 2, 6,
		1, 5, 3,
		5, 7, 3,
		0, 4, 1,
		4, 5, 1,
		2, 3, 6,
		6, 3, 7,
	};

	static const auto s_CubeVbh = bgfx::createVertexBuffer(bgfx::makeRef(s_CubeVertices, 8 * sizeof(Vertex)), Vertex::Layout);
	static const auto s_CubeIbh = bgfx::createIndexBuffer(bgfx::makeRef(s_CubeIndices, 36 * sizeof(u32)));
	
	const EntityId cube = ecs::NewEntity();

	ecs::Assign<TransformComponent>(cube, DefaultTransform());
	
	if (auto* mesh = ecs::Assign<MeshComponent>(cube))
	{
		mesh->Vbh = s_CubeVbh;
		mesh->Ibh = s_CubeIbh;
		mesh->Rph = fs::LoadProgram("base.vs", "base.fs");
	}

	return cube;
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
	bgfx::setViewTransform(0, glm::value_ptr(s_Camera->ViewMatrix()), glm::value_ptr(s_Camera->PerspectiveProjectionMatrix()));
	bgfx::setViewRect(0, 0, 0, s_Window->Width(), s_Window->Height());

	// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	bgfx::dbgTextClear();

	u8 dbgTextY = 1;
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Window size: %dx%d", s_Window->Width(), s_Window->Height());
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Run time: %.2fs", StartupTime());
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Delta time: %.2fms", (dt * 1000.0f));
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "FPS: %.2f", (1.0f / dt));
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Vsync: %s", s_Window->Vsync() ? "ON" : "OFF");

	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Camera: location %s, target %s", ToString(s_Camera->Eye).c_str(), ToString(s_Camera->At).c_str());
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Mouse: position %s", ToString(input::MousePosition()).c_str());

	for (EntityId entity : EntityFilterTemplate<TransformComponent, MeshComponent>())
	{
		bgfx::setState(BGFX_STATE_DEFAULT);
	
		const TransformComponent* transform = ecs::Get<TransformComponent>(entity);
		bgfx::setTransform(glm::value_ptr(transform->Matrix()));
	
		const MeshComponent* mesh = ecs::Get<MeshComponent>(entity);
		bgfx::setVertexBuffer(0, mesh->Vbh);
		bgfx::setIndexBuffer(mesh->Ibh);
		bgfx::submit(0, mesh->Rph);
	}

	bgfx::frame();
}
