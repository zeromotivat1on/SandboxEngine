#pragma once

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
		
    vec3    ForwardVector() const;		// calculate forward vector
    vec3    RightVector() const;		// calculate right vector
    mat4    ViewMat4() const;			// calculate view matrix
    mat4    PerspectiveMat4() const;	// calculate perspective projection matrix
    mat4    OrthographicMat4() const;   // calculate orthographic projection matrix
};

// Convert spherical coordinates (yaw, pitch) to cartesian coordinates (forward vector).
vec3 ForwardVector(f32 yaw, f32 pitch);
