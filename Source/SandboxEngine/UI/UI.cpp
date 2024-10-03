#include "sndpch.h"
#include "SandboxEngine/UI/UI.h"
#include "SandboxEngine/UI/ImguiBgfx.h"
#include "SandboxEngine/Core/Input.h"
#include <imgui/imgui.h>

static snd::Window* s_Window = nullptr;

void snd::ui::Init(Window* window)
{
	SND_ASSERT(window);

	s_Window = window;
	ImguiBgfxCreate();
}

void snd::ui::Shutdown()
{
	s_Window = nullptr;
	ImguiBgfxDestroy();
}

void snd::ui::Tick(f32 dt)
{
	const u8 imguiMouseButtons =
		(input::ButtonDown(MouseBit::Left)	 ? IMGUI_MBUT_LEFT   : 0)	|
		(input::ButtonDown(MouseBit::Right)  ? IMGUI_MBUT_RIGHT  : 0)	|
		(input::ButtonDown(MouseBit::Middle) ? IMGUI_MBUT_MIDDLE : 0);

	ImguiBgfxBeginFrame(
		input::MousePosition(),
		imguiMouseButtons,
		input::MouseScroll(),
		s_Window->Width(), s_Window->Height());

	ImGui::ShowDemoWindow();

	ImguiBgfxEndFrame();

	// TODO: looks like hack, need to think about ways to make it clearer.
	// UI consumed possible scroll input, clear it.
	input::OnMouseScroll(0.0f, 0.0f);
}
