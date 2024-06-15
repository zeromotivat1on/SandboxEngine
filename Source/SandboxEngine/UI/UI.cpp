#include "sndpch.h"
#include "SandboxEngine/UI/UI.h"
#include "SandboxEngine/bgfx-imgui/imgui_impl_bgfx.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

void snd::ui::Init(Window* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_Implbgfx_Init(0);
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetHandle(), true);
}

void snd::ui::Shutdown()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_Implbgfx_Shutdown();
	ImGui::DestroyContext();
}

void snd::ui::Tick(float dt)
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_Implbgfx_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_Implbgfx_RenderDrawData(ImGui::GetDrawData());
}
