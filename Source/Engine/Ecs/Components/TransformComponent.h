#pragma once

struct TransformComponent
{
    vec3    location;
    quat    rotation;
    vec3    scale;

    // Create transform matrix in row-major order.
    mat4    Mat4() const;
};

inline mat4 TransformComponent::Mat4() const
{
    return IdentityMat4().Scale(scale).Rotate(rotation).Translate(location);
}

// Transform with no translation and rotation, but with identity scale.
inline TransformComponent IdentityTransform()
{
    return {
        vec3(0.0f), // translation
        quat(0.0f), // rotation
        vec3(1.0f)  // scale
    };
}
