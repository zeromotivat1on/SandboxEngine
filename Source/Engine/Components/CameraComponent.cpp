﻿#include "pch.h"
#include "Components/CameraComponent.h"

vec3 CameraComponent::forward() const
{
    return vec3_forward(eye, at);
}

vec3 CameraComponent::right() const
{
    return vec3_right(eye, at, up);
}

mat4 CameraComponent::view() const
{
    return mat4_lookat(eye, at, up);
}

mat4 CameraComponent::perspective() const
{
    return mat4_perspective(gdl::rad(fov), aspect, clip_near, clip_far);
}

mat4 CameraComponent::orthographic() const
{
    return mat4_orthographic(clip_left, clip_right, clip_bottom, clip_top, clip_near, clip_far);
}