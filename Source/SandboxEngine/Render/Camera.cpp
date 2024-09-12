#include "SandboxEngine/Render/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 snd::render::Camera::ForwardVector() const
{
	return glm::normalize(At - Eye);
}

glm::vec3 snd::render::Camera::RightVector() const
{
	return glm::normalize(glm::cross(ForwardVector(), Up));
}

glm::mat4 snd::render::Camera::ViewMatrix() const
{
	return glm::lookAt(Eye, At, Up);
}

glm::mat4 snd::render::Camera::PerpsectiveProjectionMatrix() const
{
	return glm::perspective(glm::radians(Fov), Aspect, Near, Far);
}

glm::mat4 snd::render::Camera::OrthographicProjectionMatrix() const
{
	return glm::ortho(Left, Right, Bottom, Top, Near, Far);
}
