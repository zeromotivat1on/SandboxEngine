#include "pch.h"
#include "Engine/Core/EntryPoint.h"
#include "Engine/Render/Render.h"
#include "Engine/Ecs/Ecs.h"
#include <imgui/imgui.h>

// Temp test player and stuff.
#include "Ecs/Components/TransformComponent.h"
#include "Ecs/Components/MovementComponent.h"
#include "Ecs/Components/CameraComponent.h"

f32 g_dt = 0.0f;

Entity test_player_init(Ecs* ecs, Window* win)
{
	Entity player = ecs_entity_new(ecs);
    ecs_component_add(ecs, player, CT_TRANSFORM);
    ecs_component_add(ecs, player, CT_CAMERA);
    ecs_component_add(ecs, player, CT_MOVEMENT);
    
	auto* transform = new (ecs_component_get(ecs, player, CT_TRANSFORM)) TransformComponent(transform_identity());
	transform->location = vec3(0.0f, 0.0f, -50.0f);

	vec4 ortho;
    window_ortho_center(win, ortho.ptr());
        
	auto* camera = (CameraComponent*)ecs_component_get(ecs, player, CT_CAMERA);
	camera->eye  	    = transform->location;
	camera->at	 	    = vec3(0.0f);
	camera->up		    = vec3(0.0f, 1.0f, 0.0f);
	camera->yaw		    = 0.0f;
	camera->pitch	    = 0.0f;
	camera->fov		    = 60.0f;
	camera->aspect	    = window_aspect(win);
	camera->clip_near	= 0.1f;
	camera->clip_far	= 1000.0f;
	camera->clip_left	= ortho[0] * 0.1f;
	camera->clip_right	= ortho[1] * 0.1f;
	camera->clip_bottom	= ortho[2] * 0.1f;
	camera->clip_top	= ortho[3] * 0.1f;

	auto* movement = (MovementComponent*)ecs_component_get(ecs, player, CT_MOVEMENT);
	movement->velocity = vec3(0.0f);

	return player;
}

void test_player_tick(Ecs* ecs, Entity player, Entity cube, Window* win, f32 dt)
{
    static bool cursor_show = false;
    static bool cursor_lock = true;
    static bool cursor_constrain = true;
    static f32 camera_speed_scale = 1.0f;

	if (key_pressed(win, KEY_ESCAPE))
    {
    	msg_log("Window manual close by key");
    	window_close(win);
    	return;
    }

    if (key_pressed(win, KEY_F))
    {
        msg_log("Centering on given cube entity (%u)", cube);

        auto* player_transform = (TransformComponent*)ecs_component_get(ecs, player, CT_TRANSFORM);
        auto* cube_transform = (TransformComponent*)ecs_component_get(ecs, cube, CT_TRANSFORM);

        player_transform->location = cube_transform->location;
    }

    if (key_pressed(win, KEY_1))
    {
        ecs_entity_del(ecs, 1);
    }
    if (key_pressed(win, KEY_2))
    {
        ecs_entity_del(ecs, 2);
    }
    if (key_pressed(win, KEY_3))
    {
        ecs_entity_del(ecs, 3);
    }

    if (key_pressed(win, KEY_F1))
    {
        window_cursor_show(win, cursor_show);
        cursor_show = !cursor_show;
    }
    if (key_pressed(win, KEY_F2))
    {
        window_cursor_lock(win, cursor_lock);
        cursor_lock = !cursor_lock;
    }
    if (key_pressed(win, KEY_F3))
    {
        window_cursor_constrain(win, cursor_constrain);
        cursor_constrain = !cursor_constrain;
    }
    
	camera_speed_scale += gdl::sign(mouse_axis(win, MOUSE_SCROLL_Y)) * 0.2f;
	camera_speed_scale  = gdl::clamp(camera_speed_scale, 0.1f, 10.0f);

	auto* movement = (MovementComponent*)ecs_component_get(ecs, player, CT_MOVEMENT);
	auto* camera = (CameraComponent*)ecs_component_get(ecs, player, CT_CAMERA);

	const f32 mouse_sensitivity = 0.1f; // todo: mb make separate component for inputs
	const f32 pitch_limit = 89.0f;

	camera->yaw   += mouse_axis(win, MOUSE_OFFSET_X) * mouse_sensitivity;
	camera->pitch += mouse_axis(win, MOUSE_OFFSET_Y) * mouse_sensitivity;
	camera->pitch  = gdl::clamp(camera->pitch, -pitch_limit, pitch_limit);

    const f32 move_speed = 3000.0f * camera_speed_scale;
	vec3 input_velocity;

	if (key_down(win, KEY_W))
	{
		input_velocity.x += move_speed * dt;
	}

    if (key_down(win, KEY_S))
	{
		input_velocity.x -= move_speed * dt;
	}

    if (key_down(win, KEY_D))
	{
		input_velocity.y += move_speed * dt;
	}

    if (key_down(win, KEY_A))
	{
		input_velocity.y -= move_speed * dt;
	}

    if (key_down(win, KEY_E))
	{
		input_velocity.z += move_speed * dt;
	}

    if (key_down(win, KEY_Q))
	{
		input_velocity.z -= move_speed * dt;
	}

	movement->velocity =
		input_velocity.x * camera->forward() +
		input_velocity.y * camera->right()	 +
		input_velocity.z * camera->up;
}

void ecs_move_callback(Ecs* ecs, Entity e)
{
    auto* transform = (TransformComponent*)ecs_component_get(ecs, e, CT_TRANSFORM);
    auto* movement = (MovementComponent*)ecs_component_get(ecs, e, CT_MOVEMENT);

    transform->location += movement->velocity * g_dt;
}

void ecs_camera_callback(Ecs* ecs, Entity e)
{
    const auto* transform = (TransformComponent*)ecs_component_get(ecs, e, CT_TRANSFORM);
    auto* camera = (CameraComponent*)ecs_component_get(ecs, e, CT_CAMERA);

    camera->eye = transform->location;
    camera->at = camera->eye + vec3_forward(camera->yaw, camera->pitch);
}

void ecs_test_tick(Ecs* ecs, f32 dt)
{
    //SCOPE_TIMER(__FUNCTION__);
    
#if 1
    static constexpr ComponentType move_cts[] = { CT_TRANSFORM, CT_MOVEMENT };
    ecs_entity_iterate(ecs, move_cts, ARRAY_COUNT(move_cts), ecs_move_callback);

    static constexpr ComponentType camera_cts[] = { CT_TRANSFORM, CT_CAMERA };
    ecs_entity_iterate(ecs, camera_cts, ARRAY_COUNT(camera_cts), ecs_camera_callback);
#else
    for (Entity e = 0; e < ecs->max_entity_count; ++e)
    {
        auto* transform = (TransformComponent*)ecs_component_get(ecs, e, CT_TRANSFORM);
        auto* movement = (MovementComponent*)ecs_component_get(ecs, e, CT_MOVEMENT);
        auto* camera = (CameraComponent*)ecs_component_get(ecs, e, CT_CAMERA);

        if (!transform || !movement || !camera) continue;
        
        transform->location += movement->velocity * dt;

        camera->eye = transform->location;
        camera->at = camera->eye + vec3_forward(camera->yaw, camera->pitch);
    }
#endif
}

void on_window_char(Window* win, u32 character)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(character);
}

s32 EntryPoint()
{
    ASSERT(VIRT_SPACE_SIZE > PHYS_HEAP_SIZE);
    
#if BUILD_DEBUG
    void* base_addr = (void*)TB(2);
#else
    void* base_addr = nullptr;
#endif

    g_virt_space = mem_virt_reserve(base_addr, VIRT_SPACE_SIZE);
    g_phys_heap = mem_virt_commit(g_virt_space, PHYS_HEAP_SIZE);

    g_arena_persistent = arena_create(g_phys_heap, PERS_ARENA_SIZE);
    g_arena_transient = arena_create((u8*)g_phys_heap + PERS_ARENA_SIZE, TRAN_ARENA_SIZE);
    g_arena_frame = arena_create((u8*)g_phys_heap + PERS_ARENA_SIZE + TRAN_ARENA_SIZE, FRAME_ARENA_SIZE);

    path_init();
    input_tables_init();
    
    WindowInfo winfo = STRUCT_ZERO(WindowInfo);
    winfo.title = "Sandbox Engine";
    winfo.width = 1280;
    winfo.height = 720;
    winfo.x = 100;
    winfo.y = 100;
    
    Window* win = (Window*)arena_push_zero(&g_arena_persistent, WINDOW_ALLOC_SIZE);
    if (window_init(win, &winfo))
    {
        window_show(win);
        window_set_char_callback(win, on_window_char);
    }
    else
    {
        msg_critical("Failed to create window");
        return -1;
    }

    Ecs* ecs = arena_push_struct(&g_arena_persistent, Ecs);
    ecs_init(ecs, k_max_entities);

    Entity player = test_player_init(ecs, win);

    Render* r = arena_push_struct(&g_arena_persistent, Render);
    render_init(r, win, (CameraComponent*)ecs_component_get(ecs, player, CT_CAMERA), true);

	// Create debug test cubes.
    Entity cube;
    for (s32 i = 0; i < 5; ++i)
    {
		cube = ecs_entity_new_debug_cube(ecs);
		((TransformComponent*)ecs_component_get(ecs, cube, CT_TRANSFORM))->location.x += i * 10.0f;
    }

    // Stress test non-renderable entities.
#if 0
    for (u32 i = 0; i < k_max_entities / 2; ++i)
    {
		Entity test = ecs_entity_new(ecs);
		ecs_component_add(ecs, test, CT_TRANSFORM);
		ecs_component_add(ecs, test, CT_MOVEMENT);
    }
#endif
    
    g_frame_counter = 0;
	f32 dt = FPS(60);
	u64 begin_counter = hp_counter();
	while (window_active(win))
	{
        arena_clear(&g_arena_frame);

        window_update(win);
        test_player_tick(ecs, player, cube, win, dt);
        ecs_test_tick(ecs, dt);
        render_draw(r, ecs, dt);

		const u64 end_counter = hp_counter();
		g_dt = dt = (f32)(end_counter - begin_counter) / (f32)hp_frequency();
		begin_counter = end_counter;

#ifdef BUILD_DEBUG
		// If dt is too large, we must have resumed from a breakpoint, lock to the target framerate.
		if (dt > 1.0f)
		{
			dt = FPS(60);
		}
#endif

        g_frame_counter++;
	}

	render_terminate(r);
	window_destroy(win);
    mem_virt_decommit(g_phys_heap, PHYS_HEAP_SIZE);
    mem_virt_release(g_virt_space);

	return 0;
}
