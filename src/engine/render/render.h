#pragma once

struct Ecs;
struct Camera;

struct Render
{
    Window* window;
    Camera* camera;
    bool    vsync;
};

void init_render(Render* r, Window* win, Camera* cam, bool vsync);
void terminate_render(Render* r);
void draw_entities_and_ui(Render* r, Ecs* ecs, f32 dt);
void reset_render(Render* r, u16 w, u16 h);
