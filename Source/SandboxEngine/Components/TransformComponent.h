#pragma once

#include "SandboxEngine/Core/CoreMacros.h"
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>

namespace snd
{
    struct TransformComponent
    {
        glm::vec3                   Location;
        glm::quat                   Rotation;
        glm::vec3                   Scale;
        
        glm::mat4                   Matrix() const;
    };

    static_assert(std::is_pod_v<TransformComponent>);

    SND_INLINE glm::mat4 TransformComponent::Matrix() const
    {
        return glm::translate(glm::mat4(1.0f), Location) * glm::mat4_cast(Rotation) * glm::scale(glm::mat4(1.0f), Scale);
    }

    SND_INLINE TransformComponent DefaultTransform()
    {
        return
        {
            glm::vec3(),        // translation
            glm::quat(),        // rotation
            glm::vec3(1.0f)     // scale
        };
    }
}
