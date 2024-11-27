#include "pch.h"
#include "Engine/Render/Render.h"
#include "Engine/Render/Vertex.h"
#include "Engine/UI/ImguiBgfx.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Mesh.h"
#include "Engine/Components/Transform.h"
#include <bgfx/bgfx.h>

void render_init(Render* r, Window* win, Camera* cam, bool vsync)
{
    ASSERT(win);
    ASSERT(cam);

    Vertex::init_layout();
    
    r->window = win;
    r->camera = cam;
    r->vsync = vsync;
    
    bgfx::Init bgfx_init;
    bgfx_init.type = bgfx::RendererType::OpenGL;
    bgfx_init.platformData.nwh = window_native(win);

    u16 winw, winh;
    window_size_inner(win, &winw, &winh);
    
    bgfx_init.resolution.width = winw;
    bgfx_init.resolution.height = winh;

    if (vsync)
    {
	    bgfx_init.resolution.reset |= BGFX_RESET_VSYNC;
    }

    bgfx::init(bgfx_init);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);

#ifdef BUILD_DEBUG
    bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

    msg_log("Using Graphics API (%s)", bgfx::getRendererName(bgfx::getRendererType()));

    ImguiBgfxCreate();
}

void render_terminate(Render* r)
{
    memset(r, 0, sizeof(Render));
    ImguiBgfxDestroy();
    bgfx::shutdown();
}

void render_reset(Render* r, u16 w, u16 h)
{
    u32 flags = 0;

    if (r->vsync)
    {
	    flags |= BGFX_RESET_VSYNC;
    }

    bgfx::reset(w, h, flags);
}

void render_ecs_callback(ECS* ecs, Entity e)
{
    const auto* mesh = ecs_component_get_struct(ecs, e, Mesh);

    if (mesh->rph.idx > 0)
    {
        bgfx::setState(BGFX_STATE_DEFAULT);

        const auto* transform = ecs_component_get_struct(ecs, e, Transform);
        bgfx::setTransform(transform->mat4().ptr());

        bgfx::setVertexBuffer(0, mesh->vbh);
        bgfx::setIndexBuffer(mesh->ibh);
        bgfx::submit(0, mesh->rph);
    }
}

void render_draw(Render* r, ECS* ecs, f32 dt)
{
    u16 winw, winh;
    window_size_inner(r->window, &winw, &winh);

    u16 cx, cy;
    window_cursor_pos_absolute(r->window, &cx, &cy);

    u16 crx, cry;
    window_cursor_pos_relative(r->window, &crx, &cry);
    
    const s16 mx = mouse_axis(r->window, MOUSE_X);
    const s16 my = mouse_axis(r->window, MOUSE_Y);
    const s16 moffx = mouse_axis(r->window, MOUSE_OFFSET_X);
    const s16 moffy = mouse_axis(r->window, MOUSE_OFFSET_Y);
    const s16 mscrollx = mouse_axis(r->window, MOUSE_SCROLL_X);
    const s16 mscrolly = mouse_axis(r->window, MOUSE_SCROLL_Y);
    
    bgfx::setViewTransform(0, r->camera->view().ptr(), r->camera->projection().ptr());
    bgfx::setViewRect(0, 0, 0, winw, winh);

    // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    const u8 imgui_mouse_buttons =
        (mouse_down(r->window, MOUSE_LEFT)   ? IMGUI_MBUT_LEFT   : 0)   |
        (mouse_down(r->window, MOUSE_RIGHT)  ? IMGUI_MBUT_RIGHT  : 0)   |
        (mouse_down(r->window, MOUSE_MIDDLE) ? IMGUI_MBUT_MIDDLE : 0);

    ImguiBgfxBeginFrame(
        vec2(mx, my),
	    imgui_mouse_buttons,
	    vec2((f32)gdl::sign(mscrollx), (f32)gdl::sign(mscrolly)),
	    winw,
	    winh
	);
            
#ifdef BUILD_DEBUG
    bgfx::dbgTextClear();
    
    u8 debug_text_y = 1;
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Window render size: %dx%d", winw, winh);
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Frame counter: %u", g_frame_counter);
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Delta time: %.2fms", (dt * 1000.0f));
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "FPS: %.2f", (1.0f / dt));
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Vsync: %s", r->vsync ? "ON" : "OFF");

    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Camera: location (%.2f %.2f %.2f), target  (%.2f %.2f %.2f)", r->camera->eye.x, r->camera->eye.y, r->camera->eye.z, r->camera->at.x, r->camera->at.y, r->camera->at.z);
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Mouse: position (%d %d), offset (%d %d)", mx, my, moffx, moffy);
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Cursor: absolute (%u %u), relative (%u %u)", cx, cy, crx, cry);
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "ECS: entity count (%u / %u)", ecs->entity_count, ecs->max_entity_count);
#endif

    static const sid render_cts[] = { SID("Mesh") };
    ecs_entity_iterate(ecs, render_cts, ARRAY_COUNT(render_cts), render_ecs_callback);
    
    ImGui::ShowDemoWindow();

    {
        ImGui::Begin("Sandbox Debug");

        if (ImGui::TreeNode("Player"))
        {
            auto* transform = ecs_component_get_struct(ecs, 0, Transform);
            ImGui::InputFloat3("Location", transform->location.ptr());
            ImGui::InputFloat4("Rotation", transform->rotation.ptr());

            ImGui::TreePop();
        }
    
        ImGui::End();
    }

    ImguiBgfxEndFrame();
    bgfx::frame();
}
