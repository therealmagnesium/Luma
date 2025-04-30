#pragma once
#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        using Color = glm::vec3;

        namespace Colors
        {
            const glm::vec3 White = glm::vec3(1.f, 1.f, 1.f);
            const glm::vec3 Black = glm::vec3(0.f, 0.f, 0.f);
            const glm::vec3 Red = glm::vec3(1.f, 0.f, 0.f);
            const glm::vec3 Orange = glm::vec3(1.f, 0.5f, 0.2f);
            const glm::vec3 Yellow = glm::vec3(1.f, 1.f, 0.f);
            const glm::vec3 Green = glm::vec3(0.f, 1.f, 0.f);
            const glm::vec3 Cyan = glm::vec3(0.f, 1.f, 1.f);
            const glm::vec3 Blue = glm::vec3(0.f, 0.f, 1.f);
            const glm::vec3 Violet = glm::vec3(1.f, 0.f, 1.f);

            const glm::vec3 SoftWhite = glm::vec3(0.85f, 0.85f, 0.85f);
            const glm::vec3 SoftBlack = glm::vec3(0.15f, 0.15f, 0.15f);
        }
    }
}
