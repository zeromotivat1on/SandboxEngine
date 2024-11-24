#pragma once

enum CameraMode : u8
{
    CAMERA_PERSPECTIVE,
    CAMERA_ORTHOGRAPHIC
};

struct CameraComponent
{
    vec3    at;     // view point location
    vec3    up;     // vertical direction
    vec3    eye;    // camera location
        
    f32     yaw;    // horizontal rotation around y-axis
    f32     pitch;  // vertical rotation around x-axis
        
    f32 	fov;
    f32 	aspect;

    f32 	clip_near;
    f32 	clip_far;
    f32 	clip_left;
    f32 	clip_right;
    f32 	clip_bottom;
    f32 	clip_top;

    CameraMode mode;
    
    vec3    forward() const;
    vec3    right() const;
    mat4    view() const;
    mat4    projection() const;
};
