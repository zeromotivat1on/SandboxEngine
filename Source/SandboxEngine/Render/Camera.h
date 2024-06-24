#pragma once

#include <glm/glm.hpp>

namespace snd::render
{
	// Data structure to manipulate and get camera-specific data.
	struct Camera
	{
		glm::vec3 At;
		glm::vec3 Up;
		glm::vec3 Eye;

		float Fov;	// how wide camera can see
		float Near;	// how near camera can see
		float Far;	// how far camera can see

		// Calculate and get view matrix.
		glm::mat4 ViewMatrix();

		// Calculate and get perspective projection matrix.
		glm::mat4 PerpsectiveProjectionMatrix(float aspect);
		
		// Calculate and get orthographic projection matrix.
		glm::mat4 OrthographicProjectionMatrix(float l, float r, float b, float t);
		glm::mat4 OrthographicProjectionMatrix(const glm::vec4& data); // data [l, r, b, t]

		glm::vec3 ForwardVector();
		glm::vec3 RightVector();

		void MoveForward(float delta);
		void MoveBackward(float delta);
		void MoveLeft(float delta);
		void MoveRight(float delta);

		void RotateLeft(float angle);
		void RotateRight(float angle);
		void RotateUp(float angle);
		void RotateDown(float angle);
	};
}
