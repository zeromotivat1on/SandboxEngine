#include "sndpch.h"
#include "SandboxEngine/Components/TransformComponent.h"

glm::mat4 snd::TransformComponent::Matrix() const
{
    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Translation);
    const glm::mat4 rotationMatrix    = glm::mat4_cast(Rotation);
    const glm::mat4 scaleMatrix       = glm::scale(glm::mat4(1.0f), Scale);
    return translationMatrix * rotationMatrix * scaleMatrix;
}
