#pragma once

#include <glm/glm.hpp>

namespace snd::render
{
	// Data structure to manipulate and calculate camera-specific data.
	struct Camera
	{
		glm::vec3 At = {};	// the point in space the camera is looking at (target position)
		glm::vec3 Up = {};	// the up vector defining the camera's vertical direction
		glm::vec3 Eye = {};	// the position of the camera in the world space

		float Fov = 0.0f;		// the field of camera view
		float Aspect = 0.0f;	// the aspect ratio

		float Near = 0.0f;		// the camera near clipping plane
		float Far = 0.0f;		// the camera far clipping plane

		float Left = 0.0f;		// the camera left clipping plane
		float Right = 0.0f;		// the camera right clipping plane
		float Bottom = 0.0f;	// the camera bottom clipping plane
		float Top = 0.0f;		// the camera top clipping plane

		// Calculate and get view matrix.
		glm::mat4 ViewMatrix() const;

		// Calculate and get perspective projection matrix.
		glm::mat4 PerpsectiveProjectionMatrix() const;
		
		// Calculate and get orthographic projection matrix.
		glm::mat4 OrthographicProjectionMatrix() const;

		// Calculate camera forward vector.
		glm::vec3 ForwardVector() const;

		// Calculate camera right vector.
		glm::vec3 RightVector() const;
	};
}
