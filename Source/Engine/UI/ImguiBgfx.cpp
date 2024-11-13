/*
 * Copyright 2014-2015 Daniel Collin. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "ImguiBgfx.h"

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>

#include <bx/allocator.h>
#include <bx/math.h>
#include <bx/timer.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <bgfx/bgfx/examples/common/bgfx_utils.h>

#include <bgfx/bgfx/3rdparty/dear-imgui/imgui_user.h>
#include <bgfx/bgfx/3rdparty/iconfontheaders/icons_font_awesome.h>
#include <bgfx/bgfx/3rdparty/iconfontheaders/icons_kenney.h>

#include "bgfx/bgfx/examples/common/imgui/vs_ocornut_imgui.bin.h"
#include "bgfx/bgfx/examples/common/imgui/fs_ocornut_imgui.bin.h"
#include "bgfx/bgfx/examples/common/imgui/vs_imgui_image.bin.h"
#include "bgfx/bgfx/examples/common/imgui/fs_imgui_image.bin.h"

#include "bgfx/bgfx/examples/common/imgui/roboto_regular.ttf.h"
#include "bgfx/bgfx/examples/common/imgui/robotomono_regular.ttf.h"
#include "bgfx/bgfx/examples/common/imgui/icons_kenney.ttf.h"
#include "bgfx/bgfx/examples/common/imgui/icons_font_awesome.ttf.h"

static const bgfx::EmbeddedShader s_EmbeddedShaders[] =
{
	BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(vs_imgui_image),
	BGFX_EMBEDDED_SHADER(fs_imgui_image),

	BGFX_EMBEDDED_SHADER_END()
};

struct FontRangeMerge
{
	const void* Data;
	size_t      Size;
	ImWchar     Ranges[3];
};

static FontRangeMerge s_FontRangeMerge[] =
{
	{ s_iconsKenneyTtf,      sizeof(s_iconsKenneyTtf),      { ICON_MIN_KI, ICON_MAX_KI, 0 } },
	{ s_iconsFontAwesomeTtf, sizeof(s_iconsFontAwesomeTtf), { ICON_MIN_FA, ICON_MAX_FA, 0 } },
};

static void* MemAlloc(size_t size, void* userData);
static void MemFree(void* ptr, void* userData);

struct OcornutImguiContext
{
	void Render(ImDrawData* drawData)
	{
		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		const f32 fbWidth  = drawData->DisplaySize.x * drawData->FramebufferScale.x;
		const f32 fbHeight = drawData->DisplaySize.y * drawData->FramebufferScale.y;
		if (fbWidth <= 0.0f || fbHeight <= 0.0f)
			return;

		bgfx::setViewName(ViewId, "ImGui");
		bgfx::setViewMode(ViewId, bgfx::ViewMode::Sequential);

		const bgfx::Caps* caps = bgfx::getCaps();
		{
			f32 ortho[16];
			const f32 x = drawData->DisplayPos.x;
			const f32 y = drawData->DisplayPos.y;
			const f32 width  = drawData->DisplaySize.x;
			const f32 height = drawData->DisplaySize.y;

			bx::mtxOrtho(ortho, x, x + width, y + height, y, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
			bgfx::setViewTransform(ViewId, NULL, ortho);
			bgfx::setViewRect(ViewId, 0, 0, u16(width), u16(height) );
		}

		const ImVec2 clipPos   = drawData->DisplayPos;       // (0,0) unless using multi-viewports
		const ImVec2 clipScale = drawData->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

		// Render command lists
		for (s32 i = 0, num = drawData->CmdListsCount; i < num; ++i)
		{
			bgfx::TransientVertexBuffer tvb;
			bgfx::TransientIndexBuffer  tib;

			const ImDrawList* drawList = drawData->CmdLists[i];
			const u32 numVertices = static_cast<u32>(drawList->VtxBuffer.size());
			const u32 numIndices  = static_cast<u32>(drawList->IdxBuffer.size());

			if (!checkAvailTransientBuffers(numVertices, VertLayout, numIndices) )
			{
				break;
			}

			bgfx::allocTransientVertexBuffer(&tvb, numVertices, VertLayout);
			bgfx::allocTransientIndexBuffer(&tib, numIndices, sizeof(ImDrawIdx) == 4);

			ImDrawVert* verts = reinterpret_cast<ImDrawVert*>(tvb.data);
			bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

			ImDrawIdx* indices = reinterpret_cast<ImDrawIdx*>(tib.data);
			bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

			bgfx::Encoder* encoder = bgfx::begin();

			for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
			{
				if (cmd->UserCallback)
				{
					cmd->UserCallback(drawList, cmd);
				}
				else if (cmd->ElemCount > 0)
				{
					u64 state = BGFX_STATE_WRITE_RGB |
							    BGFX_STATE_WRITE_A   |
							    BGFX_STATE_MSAA;

					bgfx::TextureHandle th = Texture;
					bgfx::ProgramHandle program = Program;

					if (cmd->TextureId)
					{
						union { ImTextureID ptr; struct { bgfx::TextureHandle handle; u8 flags; u8 mip; } s; } texture = { cmd->TextureId };

						state |= IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags											?
								 BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)	:
								 BGFX_STATE_NONE;

						th = texture.s.handle;

						if (texture.s.mip != 0)
						{
							const f32 lodEnabled[4] = { static_cast<f32>(texture.s.mip), 1.0f, 0.0f, 0.0f };
							bgfx::setUniform(ImageLodEnabledUniform, lodEnabled);
							program = ImageProgram;
						}
					}
					else
					{
						state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
					}

					// Project scissor/clipping rectangles into framebuffer space
					ImVec4 clipRect;
					clipRect.x = (cmd->ClipRect.x - clipPos.x) * clipScale.x;
					clipRect.y = (cmd->ClipRect.y - clipPos.y) * clipScale.y;
					clipRect.z = (cmd->ClipRect.z - clipPos.x) * clipScale.x;
					clipRect.w = (cmd->ClipRect.w - clipPos.y) * clipScale.y;

					if (clipRect.x < fbWidth	&&
						clipRect.y < fbHeight	&&
						clipRect.z >= 0.0f		&&
						clipRect.w >= 0.0f)
					{
						const u16 xx = u16(bx::max(clipRect.x, 0.0f) );
						const u16 yy = u16(bx::max(clipRect.y, 0.0f) );
						encoder->setScissor(
							xx, yy,
							static_cast<u16>(bx::min(clipRect.z, 65535.0f)) - xx,
							static_cast<u16>(bx::min(clipRect.w, 65535.0f)) - yy
						);

						encoder->setState(state);
						encoder->setTexture(0, TextureUniform, th);
						encoder->setVertexBuffer(0, &tvb, cmd->VtxOffset, numVertices);
						encoder->setIndexBuffer(&tib, cmd->IdxOffset, cmd->ElemCount);
						encoder->submit(ViewId, program);
					}
				}
			}

			bgfx::end(encoder);
		}
	}

	void Create(f32 fontSize, bx::AllocatorI* allocator)
	{
		IMGUI_CHECKVERSION();

		Allocator = allocator;

		if (!allocator)
		{
			static bx::DefaultAllocator s_Allocator;
			Allocator = &s_Allocator;
		}

		ViewId = 255;
		LastScroll = vec2(0.0f);
		LastTime = bx::getHPCounter();

		ImGui::SetAllocatorFunctions(MemAlloc, MemFree, nullptr);

		ImguiCtx = ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(1280.0f, 720.0f);
		io.DeltaTime   = 1.0f / 60.0f;
		io.IniFilename = nullptr;

		SetupStyle(true);

		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

		const bgfx::RendererType::Enum type = bgfx::getRendererType();
		Program = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_EmbeddedShaders, type, "vs_ocornut_imgui"),
			bgfx::createEmbeddedShader(s_EmbeddedShaders, type, "fs_ocornut_imgui"),
			true
		);

		ImageLodEnabledUniform = bgfx::createUniform("ImageLodEnabledUniform", bgfx::UniformType::Vec4);
		ImageProgram = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_EmbeddedShaders, type, "vs_imgui_image"),
			bgfx::createEmbeddedShader(s_EmbeddedShaders, type, "fs_imgui_image"),
			true
		);

		VertLayout
			.begin()
			.add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
			.end();

		TextureUniform = bgfx::createUniform("TextureUniform", bgfx::UniformType::Sampler);

		u8* data	= nullptr;
		s32 width	= 0;
		s32 height	= 0;

		{
			ImFontConfig config;
			config.FontDataOwnedByAtlas = false;
			config.MergeMode = false;

			const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
			Font[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTtf,     sizeof(s_robotoRegularTtf),     fontSize,        &config, ranges);
			Font[ImGui::Font::Mono]	   = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf), fontSize - 3.0f, &config, ranges);

			config.MergeMode = true;
			config.DstFont   = Font[ImGui::Font::Regular];

			for (const auto& [Data, Size, Ranges] : s_FontRangeMerge)
			{
				io.Fonts->AddFontFromMemoryTTF(
					const_cast<void*>(Data),
					static_cast<s32>(Size),
					fontSize - 3.0f,
					&config,
					Ranges
				);
			}
		}

		io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

		Texture = bgfx::createTexture2D(
			static_cast<u16>(width), static_cast<u16>(height),
			false, 1, bgfx::TextureFormat::BGRA8, 0,
			bgfx::copy(data, width*height*4)
		);
	}

	void Destroy()
	{
		//ImGui::ShutdownDockContext();
		ImGui::DestroyContext(ImguiCtx);

		bgfx::destroy(TextureUniform);
		bgfx::destroy(Texture);

		bgfx::destroy(ImageLodEnabledUniform);
		bgfx::destroy(ImageProgram);
		bgfx::destroy(Program);

		Allocator = nullptr;
	}

	void SetupStyle(bool dark)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		if (dark)
		{
			ImGui::StyleColorsDark(&style);
		}
		else
		{
			ImGui::StyleColorsLight(&style);
		}

		style.FrameRounding    = 4.0f;
		style.WindowBorderSize = 0.0f;
	}

	void BeginFrame(const vec2& mousePos, u8 mouseButtons, const vec2& mouseScroll, u16 windowWidth, u16 windowHeight, s32 inputChar, bgfx::ViewId viewId)
	{
		ViewId = viewId;

		ImGuiIO& io = ImGui::GetIO();
		if (inputChar >= 0)
		{
			io.AddInputCharacter(inputChar);
		}

		io.DisplaySize = ImVec2(windowWidth, windowHeight);

		const s64 now = bx::getHPCounter();
		const s64 frameTime = now - LastTime;
		LastTime = now;

		const f32 freq = static_cast<f32>(bx::getHPFrequency() );
		io.DeltaTime = static_cast<f32>(frameTime) / freq;

		io.AddMousePosEvent(mousePos.x, mousePos.y);
		io.AddMouseButtonEvent(ImGuiMouseButton_Left,   mouseButtons & IMGUI_MBUT_LEFT);
		io.AddMouseButtonEvent(ImGuiMouseButton_Right,  mouseButtons & IMGUI_MBUT_RIGHT);
		io.AddMouseButtonEvent(ImGuiMouseButton_Middle, mouseButtons & IMGUI_MBUT_MIDDLE);
		io.AddMouseWheelEvent(mouseScroll.x, mouseScroll.y);
		LastScroll = mouseScroll;

		ImGui::NewFrame();
	}

	void EndFrame()
	{
		ImGui::Render();
		Render(ImGui::GetDrawData());
	}

	ImGuiContext*       	ImguiCtx;
	bx::AllocatorI*     	Allocator;
	bgfx::VertexLayout  	VertLayout;
	bgfx::ProgramHandle		Program;
	bgfx::ProgramHandle 	ImageProgram;
	bgfx::TextureHandle 	Texture;
	bgfx::UniformHandle 	TextureUniform;
	bgfx::UniformHandle 	ImageLodEnabledUniform;
	ImFont* 				Font[ImGui::Font::Count];
	s64 					LastTime;
	vec2				    LastScroll;
	bgfx::ViewId			ViewId;
};

static OcornutImguiContext s_Ctx;

static void* MemAlloc(size_t size, void* userData)
{
	return bx::alloc(s_Ctx.Allocator, size);
}

static void MemFree(void* ptr, void* userData)
{
	bx::free(s_Ctx.Allocator, ptr);
}

void ImguiBgfxCreate(f32 fontSize, bx::AllocatorI* allocator)
{
	s_Ctx.Create(fontSize, allocator);
}

void ImguiBgfxDestroy()
{
	s_Ctx.Destroy();
}

void ImguiBgfxBeginFrame(const vec2& mousePos, u8 mouseButtons, const vec2& mouseScroll, u16 windowWidth, u16 windowHeight, s32 inputChar, bgfx::ViewId view)
{
	s_Ctx.BeginFrame(mousePos, mouseButtons, mouseScroll, windowWidth, windowHeight, inputChar, view);
}

void ImguiBgfxEndFrame()
{
	s_Ctx.EndFrame();
}

namespace ImGui
{
	void PushFont(Font::Enum font)
	{
		PushFont(s_Ctx.Font[font]);
	}

	void PushEnabled(bool enabled)
	{
		PushItemFlag(ImGuiItemFlags_Disabled, !enabled);
		PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * (enabled ? 1.0f : 0.5f) );
	}

	void PopEnabled()
	{
		PopItemFlag();
		PopStyleVar();
	}
}
