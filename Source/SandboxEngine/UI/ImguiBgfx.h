#pragma once

#include <bgfx/bgfx.h>
#include <imgui/imgui.h>

#define IMGUI_MBUT_LEFT   0x01
#define IMGUI_MBUT_RIGHT  0x02
#define IMGUI_MBUT_MIDDLE 0x04

namespace snd
{
	void ImguiBgfxCreate(f32 fontSize = 18.0f, bx::AllocatorI* allocator = nullptr);
	void ImguiBgfxDestroy();

	void ImguiBgfxBeginFrame(const vec2& mousePos, u8 mouseButtons, const vec2& mouseScroll, u16 windowWidth, u16 windowHeight, i32 inputChar = -1, bgfx::ViewId view = 255);
	void ImguiBgfxEndFrame();
}

namespace ImGui
{
#define IMGUI_FLAGS_NONE        UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

	SND_INLINE ImTextureID TexId(bgfx::TextureHandle handle, u8 flags, u8 mip)
	{
		union { struct { bgfx::TextureHandle handle; u8 flags; u8 mip; } s; ImTextureID id; } tex;
		tex.s.handle = handle;
		tex.s.flags  = flags;
		tex.s.mip    = mip;
		return tex.id;
	}

	SND_INLINE void Image(
		bgfx::TextureHandle handle, u8 flags, u8 mip,
		const ImVec2& size,
		const ImVec2& uv0       = ImVec2(0.0f, 0.0f),
		const ImVec2& uv1       = ImVec2(1.0f, 1.0f),
		const ImVec4& tintCol   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
		const ImVec4& borderCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
		)
	{
		Image(TexId(handle, flags, mip), size, uv0, uv1, tintCol, borderCol);
	}

	SND_INLINE void Image(
		bgfx::TextureHandle handle,
		const ImVec2& size,
		const ImVec2& uv0       = ImVec2(0.0f, 0.0f),
		const ImVec2& uv1       = ImVec2(1.0f, 1.0f),
		const ImVec4& tintCol   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
		const ImVec4& borderCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
		)
	{
		Image(handle, IMGUI_FLAGS_ALPHA_BLEND, 0, size, uv0, uv1, tintCol, borderCol);
	}

	SND_INLINE bool ImageButton(
		bgfx::TextureHandle handle, u8 flags, u8 mip,
		const ImVec2& size,
		const ImVec2& uv0     = ImVec2(0.0f, 0.0f),
		const ImVec2& uv1     = ImVec2(1.0f, 1.0f),
		const ImVec4& bgCol   = ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
		const ImVec4& tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
		)
	{
		return ImageButton("image", TexId(handle, flags, mip), size, uv0, uv1, bgCol, tintCol);
	}

	SND_INLINE bool ImageButton(
		bgfx::TextureHandle handle,
		const ImVec2& size,
		const ImVec2& uv0     = ImVec2(0.0f, 0.0f),
		const ImVec2& uv1     = ImVec2(1.0f, 1.0f),
		const ImVec4& bgCol   = ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
		const ImVec4& tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
		)
	{
		return ImageButton(handle, IMGUI_FLAGS_ALPHA_BLEND, 0, size, uv0, uv1, bgCol, tintCol);
	}

	SND_INLINE void NextLine()
	{
		SetCursorPosY(GetCursorPosY() + GetTextLineHeightWithSpacing());
	}

	SND_INLINE bool MouseOverArea()
	{
		return ImGui::IsAnyItemActive()		|| 
			   ImGui::IsAnyItemHovered()	|| 
			   ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	}

	void PushEnabled(bool enabled);
	void PopEnabled();

}
