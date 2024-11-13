#include "pch.h"
#include "Engine/Render/Render.h"
#include "Engine/Render/Vertex.h"
#include "Engine/UI/ImguiBgfx.h"
#include "Engine/Ecs/Ecs.h"
#include "Engine/Ecs/Components/CameraComponent.h"
#include "Engine/Ecs/Components/MeshComponent.h"
#include "Engine/Ecs/Components/TransformComponent.h"
#include <bgfx/bgfx.h>

void render_init(Render* r, hwindow win, CameraComponent* cam, bool vsync)
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

void render_draw(Render* r, Ecs* ecs, f32 dt)
{
    u16 winw, winh;
    window_size_inner(r->window, &winw, &winh);

    const f32 mx = mouse_axis(r->window, MOUSE_X);
    const f32 my = mouse_axis(r->window, MOUSE_Y);
    const f32 moffx = mouse_axis(r->window, MOUSE_OFFSET_X);
    const f32 moffy = mouse_axis(r->window, MOUSE_OFFSET_Y);
    const f32 mscrollx = mouse_axis(r->window, MOUSE_SCROLL_X);
    const f32 mscrolly = mouse_axis(r->window, MOUSE_SCROLL_Y);
    
    bgfx::setViewTransform(0, r->camera->ViewMat4().Ptr(), r->camera->PerspectiveMat4().Ptr());
    bgfx::setViewRect(0, 0, 0, winw, winh);

    // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
    bgfx::touch(0);

#ifdef BUILD_DEBUG
    bgfx::dbgTextClear();
    
    u8 debug_text_y = 1;
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Window render size: %dx%d", winw, winh);
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Delta time: %.2fms", (dt * 1000.0f));
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "FPS: %.2f", (1.0f / dt));
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Vsync: %s", r->vsync ? "ON" : "OFF");

    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Camera: location (%.2f %.2f %.2f), target  (%.2f %.2f %.2f)", r->camera->eye.x, r->camera->eye.y, r->camera->eye.z, r->camera->at.x, r->camera->at.y, r->camera->at.z);
    bgfx::dbgTextPrintf(1, debug_text_y++, 0x0f, "Mouse: position (%.2f %.2f), offset (%.2f %.2f)", mx, my, moffx, moffy);
#endif

    for (Entity e = 0; e < ecs->max_entity_count; ++e)
    {
        const auto* mesh = (MeshComponent*)ecs_component(ecs, e, CT_MESH);
        if (mesh->rph.idx > 0)
        {
            bgfx::setState(BGFX_STATE_DEFAULT);

            const auto* transform = (TransformComponent*)ecs_component(ecs, e, CT_TRANSFORM);
            bgfx::setTransform(transform->Mat4().Ptr());

            bgfx::setVertexBuffer(0, mesh->vbh);
            bgfx::setIndexBuffer(mesh->ibh);
            bgfx::submit(0, mesh->rph);
        }
    }

    // User interface.
    {
        const u8 imgui_mouse_buttons =
            (mouse_down(r->window, MOUSE_LEFT)   ? IMGUI_MBUT_LEFT   : 0)   |
            (mouse_down(r->window, MOUSE_RIGHT)  ? IMGUI_MBUT_RIGHT  : 0)   |
            (mouse_down(r->window, MOUSE_MIDDLE) ? IMGUI_MBUT_MIDDLE : 0);

        ImguiBgfxBeginFrame(
            vec2(mx, my),
	    	imgui_mouse_buttons,
	    	vec2((f32)Sign(mscrollx), (f32)Sign(mscrolly)),
	    	winw,
	    	winh
	    );
        
	    ImGui::ShowDemoWindow();
        
	    ImguiBgfxEndFrame();
    }

    bgfx::frame();
}
