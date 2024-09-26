#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

namespace snd
{
    struct CameraComponent
    {
        glm::vec3 			At;		                                // point in space the camera is looking at (target position)
        glm::vec3 			Up;		                                // up vector defining the camera's vertical direction
        glm::vec3 			Eye;	                                // position of the camera in the world space
        
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
		
        glm::vec3			ForwardVector() const;					// calculate forward vector
        glm::vec3			RightVector() const;					// calculate right vector
        glm::mat4			ViewMatrix() const;						// calculate view matrix
        glm::mat4			PerspectiveProjectionMatrix() const;	// calculate perspective projection matrix
        glm::mat4			OrthographicProjectionMatrix() const;	// calculate orthographic projection matrix

        // Move camera by a given delta distance.
        void                Move(const glm::vec3& delta);
        
        // Update camera rotation based on mouse move input offset.
        void                OnMouseMove(const glm::vec2& offset, f32 sensitivity);
    };

    static_assert(std::is_pod_v<CameraComponent>);

    // Convert spherical coordinates (yaw, pitch) to cartesian coordinates (forward vector).
    glm::vec3 ForwardVector(f32 yaw, f32 pitch);
}
