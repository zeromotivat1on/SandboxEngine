#include "SandboxEngine/Render/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

glm::mat4 snd::render::Camera::ViewMatrix()
{
	return glm::lookAt(Eye, At, Up);
}

glm::mat4 snd::render::Camera::PerpsectiveProjectionMatrix(float aspect)
{
	return glm::perspective(glm::radians(Fov), aspect, Near, Far);
}

glm::mat4 snd::render::Camera::OrthographicProjectionMatrix(float l, float r, float b, float t)
{
	return glm::ortho(l, r, b, t, Near, Far);
}

glm::mat4 snd::render::Camera::OrthographicProjectionMatrix(const glm::vec4& data)
{
	return OrthographicProjectionMatrix(data[0], data[1], data[2], data[3]);
}

glm::vec3 snd::render::Camera::ForwardVector()
{
	return glm::normalize(At - Eye);
}

glm::vec3 snd::render::Camera::RightVector()
{
	return glm::normalize(glm::cross(ForwardVector(), Up));
}

void snd::render::Camera::MoveForward(float delta)
{
	const glm::vec3 forward = ForwardVector();
	Eye += forward * delta;
	At += forward * delta;
}

void snd::render::Camera::MoveBackward(float delta)
{
	MoveForward(-delta);
}

void snd::render::Camera::MoveLeft(float delta)
{
	const glm::vec3 right = RightVector();
	Eye -= right * delta;
	At -= right * delta;
}

void snd::render::Camera::MoveRight(float delta)
{
	MoveLeft(-delta);
}

void snd::render::Camera::RotateLeft(float angle)
{
	const glm::vec3 rotation = glm::rotateY(ForwardVector(), glm::radians(angle));
	At = Eye + rotation;
}

void snd::render::Camera::RotateRight(float angle)
{
	RotateLeft(-angle);
}

void snd::render::Camera::RotateUp(float angle)
{
	const glm::vec3 rotation = glm::rotate(ForwardVector(), glm::radians(angle), RightVector());
	At = Eye + rotation;
}

void snd::render::Camera::RotateDown(float angle)
{
	RotateUp(-angle);
}
