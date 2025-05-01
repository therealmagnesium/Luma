#pragma once
#include "Luma/Graphics/Shader.h"
#include "Luma/Graphics/Texture.h"
#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        struct Material
        {
            glm::vec3 albedo = glm::vec3(1.f);
            float metallic = 0.f;
            float roughness = 0.f;
            Texture* albedoTexture = NULL;
            Texture* metallicTexture = NULL;
            Texture* roughnessTexture = NULL;
            Shader* shader = NULL;
        };

        Material LoadMaterialDefault();
    }
}
