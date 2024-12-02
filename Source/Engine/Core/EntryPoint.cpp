#include "pch.h"
#include "Engine/Core/EntryPoint.h"
#include "Engine/Render/Render.h"
#include <imgui/imgui.h>

// Temp test player and stuff.
#include "Render/Vertex.h"
#include "Components/Transform.h"
#include "Components/Velocity.h"
#include "Components/Camera.h"
#include "Components/Mesh.h"
#include "Components/Material.h"

f32 g_dt = 0.0f;

Entity test_player_init(ECS* ecs, Window* win)
{
	Entity player = ecs_entity_new(ecs);
    ecs_component_add_struct(ecs, player, Transform);
    ecs_component_add_struct(ecs, player, Camera);
    ecs_component_add_struct(ecs, player, Velocity);
    
	auto* transform = ecs_component_get_struct(ecs, player, Transform);
    *transform = transform_identity();
	transform->location = vec3(0.0f, 0.0f, -10.0f);

	vec4 ortho;
    window_ortho_center(win, ortho.ptr());
        
	auto* camera = ecs_component_get_struct(ecs, player, Camera);
    camera->mode = CAMERA_PERSPECTIVE;
	camera->eye = transform->location;
	camera->at = vec3(0.0f);
	camera->up = vec3(0.0f, 1.0f, 0.0f);
	camera->yaw = 0.0f;
	camera->pitch = 0.0f;
	camera->fov = 60.0f;
	camera->aspect = window_aspect(win);
	camera->clip_near = 0.1f;
	camera->clip_far = 1000.0f;
	camera->clip_left = ortho[0];
	camera->clip_right = ortho[1];
	camera->clip_bottom	= ortho[2];
	camera->clip_top = ortho[3];

	auto* velocity = ecs_component_get_struct(ecs, player, Velocity);
	velocity->vec = vec3(0.0f);

	return player;
}

void test_player_tick(ECS* ecs, Entity player, Entity cube, Window* win, f32 dt)
{
    static bool cursor_show = false;
    static bool cursor_lock = true;
    static bool cursor_constrain = true;
    static f32 camera_speed_scale = 1.0f;
    static f32 ortho_scale = 1.0f;
    
	if (key_pressed(win, KEY_ESCAPE))
    {
    	msg_log("Window manual close by key");
    	window_close(win);
    	return;
    }

    if (key_pressed(win, KEY_F))
    {
        msg_log("Centering on given cube entity (%u)", cube);

        auto* player_transform = ecs_component_get_struct(ecs, player, Transform);
        auto* cube_transform = ecs_component_get_struct(ecs, cube, Transform);

        player_transform->location = cube_transform->location;
    }

    if (key_pressed(win, KEY_C))
    {
        auto* player_camera = ecs_component_get_struct(ecs, player, Camera);
        if (player_camera->mode == CAMERA_PERSPECTIVE)
        {
            player_camera->mode = CAMERA_ORTHOGRAPHIC;
        }
        else
        {
            player_camera->mode = CAMERA_PERSPECTIVE;
        }
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

    if (mouse_pressed(win, MOUSE_MIDDLE))
    {
        window_cursor_show(win, false);
        window_cursor_constrain(win, true);
    }
    else if (mouse_released(win, MOUSE_MIDDLE))
    {
        window_cursor_show(win, true);
        window_cursor_constrain(win, false);
    }
        
	camera_speed_scale += gdl::sign(mouse_axis(win, MOUSE_SCROLL_Y)) * 0.2f;
	camera_speed_scale  = gdl::clamp(camera_speed_scale, 0.1f, 10.0f);

	auto* velocity = ecs_component_get_struct(ecs, player, Velocity);
	auto* camera = ecs_component_get_struct(ecs, player, Camera);

	const f32 mouse_sensitivity = 0.1f; // todo: mb make separate component for inputs
	const f32 pitch_limit = 89.0f;

	camera->yaw   += mouse_axis(win, MOUSE_OFFSET_X) * mouse_sensitivity;
	camera->pitch += mouse_axis(win, MOUSE_OFFSET_Y) * mouse_sensitivity;
	camera->pitch  = gdl::clamp(camera->pitch, -pitch_limit, pitch_limit);

    const f32 move_speed = 3000.0f * camera_speed_scale;
	vec3 input_velocity;

	if (key_down(win, KEY_W))
	{
        if (camera->mode == CAMERA_PERSPECTIVE)
		    input_velocity.x += move_speed * dt;

        if (camera->mode == CAMERA_ORTHOGRAPHIC)
        {
            vec4 ortho;
            window_ortho_center(win, ortho.ptr());
    
            ortho_scale -= 0.1f * dt;
            ortho_scale = gdl::clamp(ortho_scale, 0.01f, 1.0f);

	        camera->clip_left = ortho[0] * ortho_scale;
	        camera->clip_right = ortho[1] * ortho_scale;
	        camera->clip_bottom	= ortho[2] * ortho_scale;
	        camera->clip_top = ortho[3] * ortho_scale;
        }
	}

    if (key_down(win, KEY_S))
	{
        if (camera->mode == CAMERA_PERSPECTIVE)
		    input_velocity.x -= move_speed * dt;

        if (camera->mode == CAMERA_ORTHOGRAPHIC)
        {
            vec4 ortho;
            window_ortho_center(win, ortho.ptr());
    
            ortho_scale += 0.1f * dt;
            ortho_scale = gdl::clamp(ortho_scale, 0.01f, 1.0f);

            camera->clip_left = ortho[0] * ortho_scale;
	        camera->clip_right = ortho[1] * ortho_scale;
	        camera->clip_bottom	= ortho[2] * ortho_scale;
	        camera->clip_top = ortho[3] * ortho_scale;
        }
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

	velocity->vec =
		input_velocity.x * camera->forward() +
		input_velocity.y * camera->right()	 +
		input_velocity.z * camera->up;
}

void ecs_move_callback(ECS* ecs, Entity e)
{
    auto* transform = ecs_component_get_struct(ecs, e, Transform);
    const auto* velocity = ecs_component_get_struct(ecs, e, Velocity);

    transform->location += velocity->vec * g_dt;
}

void ecs_camera_callback(ECS* ecs, Entity e)
{
    const auto* transform = ecs_component_get_struct(ecs, e, Transform);
    auto* camera = ecs_component_get_struct(ecs, e, Camera);

    camera->eye = transform->location;
    camera->at = camera->eye + vec3_forward(camera->yaw, camera->pitch);
}

void ecs_test_tick(ECS* ecs, f32 dt)
{
    //SCOPE_TIMER(__FUNCTION__);
    
#if 1
    static const sid move_cts[] = { SID("Transform"), SID("Velocity") };
    ecs_entity_iterate(ecs, move_cts, ARRAY_COUNT(move_cts), ecs_move_callback);

    static const sid camera_cts[] = { SID("Transform"), SID("Camera") };
    ecs_entity_iterate(ecs, camera_cts, ARRAY_COUNT(camera_cts), ecs_camera_callback);
#else
    for (Entity e = 0; e < ecs->max_entity_count; ++e)
    {
        auto* transform = ecs_component_get_struct(ecs, e, Transform);
        auto* velocity = ecs_component_get_struct(ecs, e, Velocity);
        auto* camera = ecs_component_get_struct(ecs, e, Camera);

        if (!transform || !velocity || !camera) continue;
        
        transform->location += velocity->vec * dt;

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

Entity ecs_entity_new_debug_cube(ECS* ecs, const char* vs, const char* fs)
{
    static const Vertex cube_vertices[24] =
    {
        // Front face
        { vec3(-1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(0.0f, 0.0f), 0xff000000 },
        { vec3( 1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(1.0f, 0.0f), 0xff0000ff },
        { vec3(-1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(0.0f, 1.0f), 0xff00ff00 },
        { vec3( 1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(1.0f, 1.0f), 0xff00ffff },

        // Back face
        { vec3(-1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(0.0f, 0.0f), 0xffff0000 },
        { vec3( 1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(1.0f, 0.0f), 0xffff00ff },
        { vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(0.0f, 1.0f), 0xffffff00 },
        { vec3( 1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(1.0f, 1.0f), 0xffffffff },

        // Left face
        { vec3(-1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(0.0f, 0.0f), 0xff000000 },
        { vec3(-1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(1.0f, 0.0f), 0xff0000ff },
        { vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(0.0f, 1.0f), 0xff00ff00 },
        { vec3(-1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(1.0f, 1.0f), 0xff00ffff },

        // Right face
        { vec3( 1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(0.0f, 0.0f), 0xffff0000 },
        { vec3( 1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(1.0f, 0.0f), 0xffff00ff },
        { vec3( 1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(0.0f, 1.0f), 0xffffff00 },
        { vec3( 1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(1.0f, 1.0f), 0xffffffff },

        // Top face
        { vec3(-1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(0.0f, 0.0f), 0xff000000 },
        { vec3( 1.0f,  1.0f, -1.0f), vec3(1.0f), vec2(1.0f, 0.0f), 0xff0000ff },
        { vec3(-1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(0.0f, 1.0f), 0xff00ff00 },
        { vec3( 1.0f,  1.0f,  1.0f), vec3(1.0f), vec2(1.0f, 1.0f), 0xff00ffff },

        // Bottom face
        { vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(0.0f, 0.0f), 0xffff0000 },
        { vec3( 1.0f, -1.0f, -1.0f), vec3(1.0f), vec2(1.0f, 0.0f), 0xffff00ff },
        { vec3(-1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(0.0f, 1.0f), 0xffffff00 },
        { vec3( 1.0f, -1.0f,  1.0f), vec3(1.0f), vec2(1.0f, 1.0f), 0xffffffff },
    };

    static const u16 cube_indices[36] =
    {
	    0,  2,  1,  1,  2,  3,
	    4,  5,  6,  5,  7,  6,
	    8,  10, 9,  9,  10, 11,
	    12, 13, 14, 13, 15, 14,
	    16, 18, 17, 17, 18, 19,
	    20, 21, 22, 21, 23, 22,
    };

    static const auto cube_vbh = bgfx::createVertexBuffer(bgfx::makeRef(cube_vertices, sizeof(cube_vertices)), Vertex::layout);
    static const auto cube_ibh = bgfx::createIndexBuffer(bgfx::makeRef(cube_indices, sizeof(cube_indices)));
    static const auto cube_rph = file_program_load(vs, fs);

    const Entity cube = ecs_entity_new(ecs);
    ecs_component_add_struct(ecs, cube, Transform);
    ecs_component_add_struct(ecs, cube, Mesh);
    
    *ecs_component_get_struct(ecs, cube, Transform) = transform_identity();

    auto* mesh = ecs_component_get_struct(ecs, cube, Mesh);
    mesh->vbh = cube_vbh;
    mesh->ibh = cube_ibh;
    mesh->rph = cube_rph;

    return cube;
}

Entity ecs_entity_new_debug_quad(ECS* ecs, const char* vs, const char* fs)
{
    static const Vertex quad_vertices[4] =
    {
        { vec3(-1.0f,  1.0f, 0.0f), vec3(1.0f), vec2(0.0f, 0.0f), 0xffffffff },
        { vec3( 1.0f,  1.0f, 0.0f), vec3(1.0f), vec2(1.0f, 0.0f), 0xffffffff },
        { vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f), vec2(0.0f, 1.0f), 0xffffffff },
        { vec3( 1.0f, -1.0f, 0.0f), vec3(1.0f), vec2(1.0f, 1.0f), 0xffffffff },
    };

    static const u16 quad_indices[6] =
    {
        0, 1, 2,
        1, 3, 2,
    };

    static const auto quad_vbh = bgfx::createVertexBuffer(bgfx::makeRef(quad_vertices, sizeof(quad_vertices)), Vertex::layout);
    static const auto quad_ibh = bgfx::createIndexBuffer(bgfx::makeRef(quad_indices, sizeof(quad_indices)));

    static const auto quad_rph = file_program_load(vs, fs);

    const Entity quad = ecs_entity_new(ecs);
    ecs_component_add_struct(ecs, quad, Transform);
    ecs_component_add_struct(ecs, quad, Mesh);

    *ecs_component_get_struct(ecs, quad, Transform) = transform_identity();

    auto* mesh = ecs_component_get_struct(ecs, quad, Mesh);
    mesh->vbh = quad_vbh;
    mesh->ibh = quad_ibh;
    mesh->rph = quad_rph;

    return quad;
}

s32 entry_point()
{
    constexpr u64 k_virt_space_size = GB(8);
    constexpr u64 k_arena_persistent_size = MB(16);
    constexpr u64 k_arena_transient_size = GB(1);
    constexpr u64 k_arena_frame_size = MB(4);
    constexpr u64 k_phys_heap_size = k_arena_persistent_size + k_arena_transient_size + k_arena_frame_size;

    ASSERT(k_virt_space_size > k_phys_heap_size);    
    
#if BUILD_DEBUG
    void* base_addr = (void*)TB(2);
#else
    void* base_addr = nullptr;
#endif
    
    g_virt_space = vm_reserve(base_addr, k_virt_space_size);
    g_phys_heap = vm_commit(g_virt_space, k_phys_heap_size);

    g_arena_persistent = arena_create(g_phys_heap, k_arena_persistent_size);
    g_arena_transient = arena_create((u8*)g_phys_heap + k_arena_persistent_size, k_arena_transient_size);
    g_arena_frame = arena_create((u8*)g_phys_heap + k_arena_persistent_size + k_arena_transient_size, k_arena_frame_size);

    constexpr u32 k_sid_max_count = 2048;
    constexpr u32 k_sid_max_size = 64;
    sid_init(&g_arena_transient, k_sid_max_count, k_sid_max_size);
    
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

    constexpr u32 k_max_entities = 1024 * 1024;
    constexpr u32 k_max_component_types = 1024;
    
    ECS* ecs = arena_push_struct(&g_arena_persistent, ECS);
    ecs_init(ecs, &g_arena_transient, k_max_entities, k_max_component_types);
    ecs_component_reg_struct(ecs, &g_arena_transient, Transform);
    ecs_component_reg_struct(ecs, &g_arena_transient, Velocity);
    ecs_component_reg_struct(ecs, &g_arena_transient, Camera);
    ecs_component_reg_struct(ecs, &g_arena_transient, Mesh);
    ecs_component_reg_struct(ecs, &g_arena_transient, Material);
    
    Entity player = test_player_init(ecs, win);

    Render* r = arena_push_struct(&g_arena_persistent, Render);
    render_init(r, win, ecs_component_get_struct(ecs, player, Camera), true);

	// Create debug test cubes.
    Entity cube;
    for (s32 i = 0; i < 5; ++i)
    {
		cube = ecs_entity_new_debug_cube(ecs, "bin/cube_color.vs.bin", "bin/cube_color.fs.bin");
		(ecs_component_get_struct(ecs, cube, Transform))->location.x += i * 10.0f;
    }

    const Entity quad = ecs_entity_new_debug_quad(ecs, "bin/player.vs.bin", "bin/player.fs.bin");
    ecs_component_add_struct(ecs, quad, Material);
    auto* quad_mat = ecs_component_get_struct(ecs, quad, Material);
    quad_mat->uniform = bgfx::createUniform("s_tex_color", bgfx::UniformType::Sampler);
    quad_mat->texture = file_texture_load("bin/petscope_idle_down.dds");
	(ecs_component_get_struct(ecs, quad, Transform))->location.z -= 5.0f;
    
    // Stress test non-renderable entities.
#if 0
    for (u32 i = 0; i < k_max_entities / 8; ++i)
    {
		Entity test = ecs_entity_new(ecs);
		ecs_component_add_struct(ecs, test, Transform);
		ecs_component_add_struct(ecs, test, Velocity);
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
    vm_decommit(g_phys_heap, k_phys_heap_size);
    vm_release(g_virt_space);

	return 0;
}
