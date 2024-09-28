#include "sndpch.h"
#include "SandboxEngine/Components/CameraComponent.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 snd::CameraComponent::ForwardVector() const
{
    return glm::normalize(At - Eye);
}

glm::vec3 snd::CameraComponent::RightVector() const
{
    return glm::normalize(glm::cross(ForwardVector(), Up));
}

glm::mat4 snd::CameraComponent::ViewMatrix() const
{
    return glm::lookAt(Eye, At, Up);
}

glm::mat4 snd::CameraComponent::PerspectiveProjectionMatrix() const
{
    return glm::perspective(glm::radians(Fov), Aspect, Near, Far);
}

glm::mat4 snd::CameraComponent::OrthographicProjectionMatrix() const
{
    return glm::ortho(Left, Right, Bottom, Top, Near, Far);
}

glm::vec3 snd::ForwardVector(f32 yaw, f32 pitch)
{
    const f32 ycos = cosf(glm::radians(yaw)); 
    const f32 ysin = sinf(glm::radians(yaw));
    const f32 pcos = cosf(glm::radians(pitch));
    const f32 psin = sinf(glm::radians(pitch));
    const glm::vec3 forward = { ycos * pcos, psin, ysin * pcos };
    return glm::normalize(forward);
}
