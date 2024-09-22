#pragma once

#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>

namespace snd
{
    struct TransformComponent
    {
        glm::vec3       Translation;
        glm::quat       Rotation;
        glm::vec3       Scale;
        
        glm::mat4       Matrix() const;     // calculate transform 4x4 matrix
    };
}
