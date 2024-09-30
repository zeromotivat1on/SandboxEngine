#pragma once

namespace snd
{
    struct TransformComponent
    {
        vec3        Location;
        quat        Rotation;
        vec3        Scale;
        
        mat4        Mat4() const;
    };

    // Create transform matrix in row-major order.
    SND_INLINE mat4 TransformComponent::Mat4() const
    {
        return IdentityMat4().Scale(Scale) * Rotation.Mat4() * IdentityMat4().Translate(Location);
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
