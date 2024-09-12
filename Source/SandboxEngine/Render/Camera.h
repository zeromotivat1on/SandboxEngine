#pragma once

#include <glm/glm.hpp>

namespace snd::render
{
	// Data structure to manipulate and calculate camera-specific data.
	struct Camera
	{
		glm::vec3 			At;		// the point in space the camera is looking at (target position)
		glm::vec3 			Up;		// the up vector defining the camera's vertical direction
		glm::vec3 			Eye;	// the position of the camera in the world space

		float 				Fov;	// the field of camera view
		float 				Aspect;	// the aspect ratio

		float 				Near;	// the camera near clipping plane
		float 				Far;	// the camera far clipping plane

		float 				Left;	// the camera left clipping plane
		float 				Right;	// the camera right clipping plane
		float 				Bottom;	// the camera bottom clipping plane
		float 				Top;	// the camera top clipping plane
		
		glm::vec3			ForwardVector() const;					// calculate camera forward vector
		glm::vec3			RightVector() const;					// calculate camera right vector
		glm::mat4			ViewMatrix() const;						// calculate and get view matrix
		glm::mat4			PerpsectiveProjectionMatrix() const;	// calculate and get perspective projection matrix
		glm::mat4			OrthographicProjectionMatrix() const;	// calculate and get orthographic projection matrix
	};
}
