#include "sndpch.h"
#include "SandboxEngine/Render/Render.h"
#include "SandboxEngine/Render/Vertex.h"
#include "SandboxEngine/UI/ImguiBgfx.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Filesystem/Filesystem.h"
#include "SandboxEngine/Components/CameraComponent.h"
#include "SandboxEngine/Components/MeshComponent.h"
#include "SandboxEngine/Components/TransformComponent.h"
#include "SandboxEngine/Ecs/Ecs.h"
#include "SandboxEngine/Ecs/EntityFilter.h"
#include <bgfx/bgfx.h>

void snd::Renderer::Init()
{
    SND_ASSERT(Window);

	bgfx::Init bgfxInit;
	bgfxInit.type = bgfx::RendererType::OpenGL;
	bgfxInit.platformData.nwh = Window->NativeHandle;
	bgfxInit.resolution.width = Window->Width;
	bgfxInit.resolution.height = Window->Height;

	if (Vsync)
	{
		bgfxInit.resolution.reset |= BGFX_RESET_VSYNC;
	}

	bgfx::init(bgfxInit);

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);

#ifdef SND_BUILD_DEBUG
	bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

	SND_LOG("Using Graphics API (%s)", bgfx::getRendererName(bgfx::getRendererType()));

	Vertex::InitLayout();

	ImguiBgfxCreate();
}

void snd::Renderer::Terminate()
{
    ImguiBgfxDestroy();
	bgfx::shutdown();
}

void snd::Renderer::Reset(u16 width, u16 height)
{
	u32 flags = 0;

	if (Vsync)
	{
		flags |= BGFX_RESET_VSYNC;
	}

	bgfx::reset(width, height, flags);
}

void snd::Renderer::Render(f32 dt)
{
    SND_ASSERT(Camera);

	bgfx::setViewTransform(0, Camera->ViewMat4().Ptr(), Camera->PerspectiveMat4().Ptr());
	bgfx::setViewRect(0, 0, 0, Window->Width, Window->Height);

	// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
	bgfx::touch(0);

#ifdef SND_BUILD_DEBUG
	bgfx::dbgTextClear();

	u8 dbgTextY = 1;
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Window size: %dx%d", Window->Width, Window->Height);
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Delta time: %.2fms", (dt * 1000.0f));
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "FPS: %.2f", (1.0f / dt));
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Vsync: %s", Vsync ? "ON" : "OFF");

	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Camera: location %s, target %s", Camera->Eye.String(), Camera->At.String());
	bgfx::dbgTextPrintf(1, dbgTextY++, 0x0f, "Mouse: position %s, offset %s", Window->MousePos().String(), Window->MouseOffset().String());
#endif

	for (Entity entity : EntityFilterTemplate<TransformComponent, MeshComponent>())
	{
		bgfx::setState(BGFX_STATE_DEFAULT);

		const TransformComponent* transform = GetComponent<TransformComponent>(entity);
		bgfx::setTransform(transform->Mat4().Ptr());

		const MeshComponent* mesh = GetComponent<MeshComponent>(entity);
		bgfx::setVertexBuffer(0, mesh->Vbh);
		bgfx::setIndexBuffer(mesh->Ibh);
		bgfx::submit(0, mesh->Rph);
	}

    // User interface.
    {
        const u8 imguiMouseButtons =
    	   (Window->Buttons[INPUT_MOUSE_LEFT] == INPUT_PRESS   ? IMGUI_MBUT_LEFT   : 0)	|
    	   (Window->Buttons[INPUT_MOUSE_RIGHT] == INPUT_PRESS  ? IMGUI_MBUT_RIGHT  : 0)	|
    	   (Window->Buttons[INPUT_MOUSE_MIDDLE] == INPUT_PRESS ? IMGUI_MBUT_MIDDLE : 0);

        ImguiBgfxBeginFrame(
        	Window->MousePos(),
        	imguiMouseButtons,
        	Window->ScrollOffset(),
        	Window->Width,
        	Window->Height
        );

        ImGui::ShowDemoWindow();

        ImguiBgfxEndFrame();
    }

	bgfx::frame();
}

snd::Entity snd::NewEntityDebugCube()
{
	static const Vertex cubeVertices[8] =
	{
		{ vec3(-1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(0.0f), 0xff000000 },
		{ vec3( 1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(0.0f), 0xff0000ff },
		{ vec3(-1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(0.0f), 0xff00ff00 },
		{ vec3( 1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(0.0f), 0xff00ffff },
		{ vec3(-1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(0.0f), 0xffff0000 },
		{ vec3( 1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(0.0f), 0xffff00ff },
		{ vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(0.0f), 0xffffff00 },
		{ vec3( 1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(0.0f), 0xffffffff },
	};

	static const u16 cubeIndices[36] =
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

	static const auto cubeVbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, 8 * sizeof(Vertex)), Vertex::Layout);
	static const auto cubeIbh = bgfx::createIndexBuffer(bgfx::makeRef(cubeIndices, 36 * sizeof(u32)));
	static const auto cubeRph = ReadProgram("base.vs.bin", "base.fs.bin");

	const Entity cube = NewEntity();

	AssignComponent<TransformComponent>(cube, IdentityTransform());

	if (auto* mesh = AssignComponent<MeshComponent>(cube))
	{
		mesh->Vbh = cubeVbh;
		mesh->Ibh = cubeIbh;
		mesh->Rph = cubeRph;
	}

	return cube;
}
