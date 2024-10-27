#pragma once

namespace snd
{
    struct TransformComponent
    {
        vec3        Location;
        quat        Rotation;
        vec3        Scale;

        // Create transform matrix in row-major order.
        mat4        Mat4() const;
    };

    SND_INLINE mat4 TransformComponent::Mat4() const
    {
        return IdentityMat4().Scale(Scale).Rotate(Rotation).Translate(Location);
    }

    // Transform with no translation and rotation, but with identity scale.
    SND_INLINE TransformComponent IdentityTransform()
    {
        return
        {
            vec3(0.0f),     // translation
            quat(0.0f),     // rotation
            vec3(1.0f)      // scale
        };
    }
}
