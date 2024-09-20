#include "sndpch.h"
#include "SandboxEngine/UI/UI.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/bgfx-imgui/imgui_impl_bgfx.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

static snd::Window* s_Window = nullptr;

void snd::ui::Init(Window* window)
{
	s_Window = window;
	SND_ASSERT(s_Window);
	
	IMGUI_CHECKVERSION();
	SND_ASSERT(ImGui::CreateContext());
	SND_ASSERT(ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window->GetHandle()), true));
	ImGui_Implbgfx_Init(0);
}

void snd::ui::Shutdown()
{
	s_Window = nullptr;
	
	ImGui_ImplGlfw_Shutdown();
	ImGui_Implbgfx_Shutdown();
	ImGui::DestroyContext();
}

void snd::ui::Tick(f32 dt)
{
	ImGui_Implbgfx_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();
	
	ImGui::Render();
	ImGui_Implbgfx_RenderDrawData(ImGui::GetDrawData());
}
