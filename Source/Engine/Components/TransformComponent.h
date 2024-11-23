#pragma once

struct TransformComponent
{
    vec3    location;
    quat    rotation;
    vec3    scale;

    mat4    mat4() const;
};

inline mat4 TransformComponent::mat4() const
{
    return mat4_identity().scale(scale).rotate(rotation).translate(location);
}

// Transform with no translation and rotation, but with identity scale.
inline TransformComponent transform_identity()
{
    return {
        vec3(0.0f), // translation
        quat(0.0f), // rotation
        vec3(1.0f)  // scale
    };
}
