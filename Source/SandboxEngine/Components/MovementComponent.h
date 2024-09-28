#pragma once

#include <glm/vec3.hpp>

namespace snd
{
    struct MovementComponent
    {
        glm::vec3       Velocity;
    };

    static_assert(std::is_pod_v<MovementComponent>);
}
