#pragma once

struct ECS;
struct CameraComponent;

struct Render
{
    Window*             window;
    CameraComponent*    camera;
    bool                vsync;
};

void render_init(Render* r, Window* win, CameraComponent* cam, bool vsync);
void render_terminate(Render* r);
void render_draw(Render* r, ECS* ecs, f32 dt);
void render_reset(Render* r, u16 w, u16 h);
