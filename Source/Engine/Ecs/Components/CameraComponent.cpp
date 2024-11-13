#include "pch.h"
#include "Ecs/Components/CameraComponent.h"

vec3 CameraComponent::ForwardVector() const
{
    return (at - eye).Normalize();
}

vec3 CameraComponent::RightVector() const
{
    return ForwardVector().Cross(up).Normalize();
}

mat4 CameraComponent::ViewMat4() const
{
    return lookat(eye, at, up);
}

mat4 CameraComponent::PerspectiveMat4() const
{
    return perspective(Rad(fov), aspect, clip_near, clip_far);
}

mat4 CameraComponent::OrthographicMat4() const
{
    return orthographic(clip_left, clip_right, clip_bottom, clip_top, clip_near, clip_far);
}

vec3 ForwardVector(f32 yaw, f32 pitch)
{
    const f32 ycos = Cos(Rad(yaw)); 
    const f32 ysin = Sin(Rad(yaw));
    const f32 pcos = Cos(Rad(pitch));
    const f32 psin = Sin(Rad(pitch));
    return vec3(ycos * pcos, psin, ysin * pcos).Normalize();
}
