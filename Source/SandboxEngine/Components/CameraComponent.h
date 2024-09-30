#pragma once

namespace snd
{
    struct CameraComponent
    {
        vec3 			    At;		                                // point in space the camera is looking at (target position)
        vec3 			    Up;		                                // up vector defining the camera's vertical direction
        vec3 			    Eye;	                                // position of the camera in the world space
        
        f32                 Yaw;                                    // horizontal rotation around Y axis
        f32                 Pitch;                                  // vertical rotation around X axis
        
        f32 				Fov;	                                // field of camera view
        f32 				Aspect;	                                // aspect ratio
        
        f32 				Near;	                                // near clipping plane
        f32 				Far;	                                // far clipping plane
        f32 				Left;	                                // left clipping plane
        f32 				Right;	                                // right clipping plane
        f32 				Bottom;	                                // bottom clipping plane
        f32 				Top;	                                // top clipping plane
		
        vec3			    ForwardVector() const;					// calculate forward vector
        vec3			    RightVector() const;					// calculate right vector
        mat4			    ViewMat4() const;						// calculate view matrix
        mat4			    PerspectiveMat4() const;	// calculate perspective projection matrix
        mat4			    OrthographicMat4() const;	// calculate orthographic projection matrix
    };

    // Convert spherical coordinates (yaw, pitch) to cartesian coordinates (forward vector).
    vec3 ForwardVector(f32 yaw, f32 pitch);
}
