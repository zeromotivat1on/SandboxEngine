#pragma once

struct ECS;
struct Camera;

struct Render
{
    Window* window;
    Camera* camera;
    bool    vsync;
};

void render_init(Render* r, Window* win, Camera* cam, bool vsync);
void render_terminate(Render* r);
void render_draw(Render* r, ECS* ecs, f32 dt);
void render_reset(Render* r, u16 w, u16 h);
