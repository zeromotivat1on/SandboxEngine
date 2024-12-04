#include "pch.h"
#include "core/entry_point.h"
#include "render/render.h"
#include <imgui/imgui.h>

// Temp test player and stuff.
#include "render/vertex.h"
#include "components/transform.h"
#include "components/velocity.h"
#include "components/camera.h"
#include "components/mesh.h"
#include "components/texture.h"
#include "components/spring_arm.h"

f32 g_dt = 0.0f;

Entity test_player_init(Ecs* ecs, Window* win)
{
	Entity player = new_entity(ecs);
    add_component_struct(ecs, player, Transform);
    add_component_struct(ecs, player, Velocity);
    add_component_struct(ecs, player, Camera);
    add_component_struct(ecs, player, Spring_Arm);
    add_component_struct(ecs, player, Mesh);
    add_component_struct(ecs, player, Texture);
    
	auto* transform = get_component_struct(ecs, player, Transform);
    *transform = transform_identity();
	transform->location = vec3(0.0f, 0.0f, 0.1f);

	auto* velocity = get_component_struct(ecs, player, Velocity);
	velocity->vec = vec3(0.0f);
    
	vec4 ortho;
    window_ortho_center(win, ortho.ptr());
        
	auto* camera = get_component_struct(ecs, player, Camera);
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

	auto* spring_arm = get_component_struct(ecs, player, Spring_Arm);
    spring_arm->offset = vec3(0.0f, 0.0f, -10.0f);
    
	return player;
}

void test_player_tick(Ecs* ecs, Entity player, Entity cube, Window* win, f32 dt)
{
    static bool cursor_show = false;
    static bool cursor_lock = true;
    static bool cursor_constrain = true;
    static f32 camera_speed_scale = 1.0f;
    static f32 ortho_scale = 1.0f;
    
	if (key_pressed(win, KEY_ESCAPE))
    {
    	msg_log("Window manual close by key");
    	close_window(win);
    	return;
    }

    if (key_pressed(win, KEY_F))
    {
        msg_log("Centering on given cube entity (%u)", cube);

        auto* player_transform = get_component_struct(ecs, player, Transform);
        auto* cube_transform = get_component_struct(ecs, cube, Transform);

        player_transform->location = cube_transform->location;
    }

    if (key_pressed(win, KEY_C))
    {
        auto* player_camera = get_component_struct(ecs, player, Camera);
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
        delete_entity(ecs, 1);
    }
    if (key_pressed(win, KEY_2))
    {
        delete_entity(ecs, 2);
    }
    if (key_pressed(win, KEY_3))
    {
        delete_entity(ecs, 3);
    }

    if (key_pressed(win, KEY_F1))
    {
        show_window_cursor(win, cursor_show);
        cursor_show = !cursor_show;
    }
    if (key_pressed(win, KEY_F2))
    {
        lock_window_cursor(win, cursor_lock);
        cursor_lock = !cursor_lock;
    }
    if (key_pressed(win, KEY_F3))
    {
        constrain_window_cursor(win, cursor_constrain);
        cursor_constrain = !cursor_constrain;
    }

    if (mouse_pressed(win, MOUSE_MIDDLE))
    {
        show_window_cursor(win, false);
        constrain_window_cursor(win, true);
    }
    else if (mouse_released(win, MOUSE_MIDDLE))
    {
        show_window_cursor(win, true);
        constrain_window_cursor(win, false);
    }
        
	camera_speed_scale += gdl::sign(mouse_axis(win, MOUSE_SCROLL_Y)) * 0.2f;
	camera_speed_scale  = gdl::clamp(camera_speed_scale, 0.1f, 10.0f);

	auto* velocity = get_component_struct(ecs, player, Velocity);
	auto* camera = get_component_struct(ecs, player, Camera);

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

void ecs_move_callback(Ecs* ecs, Entity e)
{
    auto* transform = get_component_struct(ecs, e, Transform);
    const auto* velocity = get_component_struct(ecs, e, Velocity);

    transform->location += velocity->vec * g_dt;
}

void ecs_camera_callback(Ecs* ecs, Entity e)
{
    auto* camera = get_component_struct(ecs, e, Camera);
    const auto* transform = get_component_struct(ecs, e, Transform);
    
    camera->eye = transform->location;

    if (const auto* spring_arm = get_component_struct(ecs, e, Spring_Arm))
    {
        camera->eye += spring_arm->offset;
    }

    camera->at = camera->eye + vec3_forward(camera->yaw, camera->pitch);
}

void ecs_test_tick(Ecs* ecs, f32 dt)
{
    //SCOPE_TIMER(__FUNCTION__);
    
#if 1
    static const sid move_cts[] = { SID("Transform"), SID("Velocity") };
    iterate_entities(ecs, move_cts, ARRAY_COUNT(move_cts), ecs_move_callback);

    static const sid camera_cts[] = { SID("Transform"), SID("Camera") };
    iterate_entities(ecs, camera_cts, ARRAY_COUNT(camera_cts), ecs_camera_callback);
#else
    for (Entity e = 0; e < ecs->max_entity_count; ++e)
    {
        auto* transform = get_component_struct(ecs, e, Transform);
        auto* velocity = get_component_struct(ecs, e, Velocity);
        auto* camera = get_component_struct(ecs, e, Camera);

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

Entity new_entity_debug_cube(Ecs* ecs, const char* vs, const char* fs)
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
    static const auto cube_rph = load_program(vs, fs);

    const Entity cube = new_entity(ecs);
    add_component_struct(ecs, cube, Transform);
    add_component_struct(ecs, cube, Mesh);
    
    *get_component_struct(ecs, cube, Transform) = transform_identity();

    auto* mesh = get_component_struct(ecs, cube, Mesh);
    mesh->vbh = cube_vbh;
    mesh->ibh = cube_ibh;
    mesh->rph = cube_rph;

    return cube;
}

void mesh_quad(Mesh* mesh, const char* vs, const char* fs)
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

    static const auto quad_rph = load_program(vs, fs);

    mesh->vbh = quad_vbh;
    mesh->ibh = quad_ibh;
    mesh->rph = quad_rph;
}

Entity new_entity_debug_quad(Ecs* ecs, const char* vs, const char* fs)
{
    const Entity quad = new_entity(ecs);
    add_component_struct(ecs, quad, Transform);
    add_component_struct(ecs, quad, Mesh);

    *get_component_struct(ecs, quad, Transform) = transform_identity();
    mesh_quad(get_component_struct(ecs, quad, Mesh), vs, fs);

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
    init_sid(&g_arena_transient, k_sid_max_count, k_sid_max_size);
    
    init_path();
    init_input_tables();
    
    Window_Info winfo = STRUCT_ZERO(Window_Info);
    winfo.title = "Sandbox Engine";
    winfo.width = 1280;
    winfo.height = 720;
    winfo.x = 100;
    winfo.y = 100;
    
    Window* win = (Window*)arena_push_zero(&g_arena_persistent, WINDOW_ALLOC_SIZE);
    if (init_window(win, &winfo))
    {
        show_window(win);
        set_window_char_callback(win, on_window_char);
    }
    else
    {
        msg_critical("Failed to create window");
        return -1;
    }

    constexpr u32 k_max_entities = 1024 * 1024;
    constexpr u32 k_max_component_types = 1024;
    
    Ecs* ecs = arena_push_struct(&g_arena_persistent, Ecs);
    init_ecs(ecs, &g_arena_transient, k_max_entities, k_max_component_types);

    register_component_struct(ecs, &g_arena_transient, Transform);
    register_component_struct(ecs, &g_arena_transient, Velocity);
    register_component_struct(ecs, &g_arena_transient, Camera);
    register_component_struct(ecs, &g_arena_transient, Mesh);
    register_component_struct(ecs, &g_arena_transient, Texture);
    register_component_struct(ecs, &g_arena_transient, Spring_Arm);
    
    Entity player = test_player_init(ecs, win);

    Render* r = arena_push_struct(&g_arena_persistent, Render);
    init_render(r, win, get_component_struct(ecs, player, Camera), true);

    // Add player texture.
    auto* texture = get_component_struct(ecs, player, Texture);
    texture->uniform = bgfx::createUniform("s_tex_color", bgfx::UniformType::Sampler);
    texture->handle = load_texture("bin/petscope_idle_down.dds");

    // Add mesh quad to set texture on.
    mesh_quad(get_component_struct(ecs, player, Mesh), "bin/player.vs.bin", "bin/player.fs.bin");

    const Entity quad = new_entity_debug_quad(ecs, "bin/player.vs.bin", "bin/player.fs.bin");
    add_component_struct(ecs, quad, Texture);
    auto* quad_mat = get_component_struct(ecs, quad, Texture);
    quad_mat->uniform = bgfx::createUniform("s_tex_color", bgfx::UniformType::Sampler);
    quad_mat->handle = load_texture("bin/petscope_idle_down.dds");
	(get_component_struct(ecs, quad, Transform))->location.z -= 5.0f;
    
	// Create debug test cubes.
    Entity cube;
    for (s32 i = 0; i < 5; ++i)
    {
		cube = new_entity_debug_cube(ecs, "bin/cube_color.vs.bin", "bin/cube_color.fs.bin");
		(get_component_struct(ecs, cube, Transform))->location.x += i * 10.0f;
    }
    
    // Stress test non-renderable entities.
#if 0
    for (u32 i = 0; i < k_max_entities / 8; ++i)
    {
		Entity test = new_entity(ecs);
		add_component_struct(ecs, test, Transform);
		add_component_struct(ecs, test, Velocity);
    }
#endif
    
    g_frame_counter = 0;
	f32 dt = FPS(60);
	u64 begin_counter = performance_counter();
	while (is_window_active(win))
	{
        arena_clear(&g_arena_frame);

        update_window(win);
        test_player_tick(ecs, player, cube, win, dt);
        ecs_test_tick(ecs, dt);
        draw_entities_and_ui(r, ecs, dt);

		const u64 end_counter = performance_counter();
		g_dt = dt = (f32)(end_counter - begin_counter) / (f32)performance_frequency();
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

	terminate_render(r);
	destroy_window(win);
    vm_decommit(g_phys_heap, k_phys_heap_size);
    vm_release(g_virt_space);

	return 0;
}
