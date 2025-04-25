#pragma once
#include "Luma/Graphics/Shader.h"
#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        struct Attenuation
        {
            float constant;
            float linear;
            float quadratic;
        };

        struct DirectionalLight
        {
            float intensity = 1.f;
            glm::vec3 color = glm::vec3(1.f);
            glm::vec3 direction = glm::vec3(0.f, -1.f, 0.f);
        };

        struct PointLight
        {
            float intensity = 1.f;
            glm::vec3 color = glm::vec3(1.f);
            glm::vec3 position = glm::vec3(0.f);
            Attenuation attenuation;
        };

        struct SpotLight
        {
            float intensity = 1.f;
            float innerRadius = 0.f;
            float outerRadius = 0.f;
            glm::vec3 position = glm::vec3(0.f);
            glm::vec3 target = glm::vec3(0.f);
            glm::vec3 color = glm::vec3(1.f);
        };

        void DrawLight(DirectionalLight& sun, Shader& shader);
        void DrawLight(PointLight& pointLight, Shader& shader);
        void DrawLight(SpotLight& spotLight, Shader& shader);
    }
}
