#pragma once

enum CameraMode : u8
{
    CAMERA_PERSPECTIVE,
    CAMERA_ORTHOGRAPHIC
};

struct Camera
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

inline vec3 Camera::forward() const
{
    return vec3_forward(eye, at);
}

inline vec3 Camera::right() const
{
    return vec3_right(eye, at, up);
}

inline mat4 Camera::view() const
{
    return mat4_lookat(eye, at, up);
}

inline mat4 Camera::projection() const
{
    if (mode == CAMERA_PERSPECTIVE)
        return mat4_perspective(gdl::rad(fov), aspect, clip_near, clip_far);

    if (mode == CAMERA_ORTHOGRAPHIC)
        return mat4_orthographic(clip_left, clip_right, clip_bottom, clip_top, clip_near, clip_far);

    msg_error("Unknown camera mode (%u)", mode);
    return mat4_identity();    
}
