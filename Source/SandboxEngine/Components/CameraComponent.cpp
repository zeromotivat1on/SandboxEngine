#include "sndpch.h"
#include "SandboxEngine/Components/CameraComponent.h"

snd::vec3 snd::CameraComponent::ForwardVector() const
{
    return (At - Eye).Normalize();
}

snd::vec3 snd::CameraComponent::RightVector() const
{
    return ForwardVector().Cross(Up).Normalize();
}

snd::mat4 snd::CameraComponent::ViewMat4() const
{
    return math::LookAt(Eye, At, Up);
}

snd::mat4 snd::CameraComponent::PerspectiveMat4() const
{
    return math::Perspective(math::Rad(Fov), Aspect, Near, Far);
}

snd::mat4 snd::CameraComponent::OrthographicMat4() const
{
    return math::Orthographic(Left, Right, Bottom, Top, Near, Far);
}

snd::vec3 snd::ForwardVector(f32 yaw, f32 pitch)
{
    const f32 ycos = math::Cos(math::Rad(yaw)); 
    const f32 ysin = math::Sin(math::Rad(yaw));
    const f32 pcos = math::Cos(math::Rad(pitch));
    const f32 psin = math::Sin(math::Rad(pitch));
    return vec3(ycos * pcos, psin, ysin * pcos).Normalize();
}
