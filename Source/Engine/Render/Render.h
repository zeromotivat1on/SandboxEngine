#pragma once

struct Ecs;
struct CameraComponent;

struct Render
{
    hwindow             window;
    CameraComponent*    camera;
    bool                vsync;
};

void render_init(Render* r, hwindow win, CameraComponent* cam, bool vsync);
void render_terminate(Render* r);
void render_draw(Render* r, Ecs* ecs, f32 dt);
void render_reset(Render* r, u16 w, u16 h);
