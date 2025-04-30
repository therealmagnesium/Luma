#include "Luma/Graphics/Lights.h"
#include "Luma/Graphics/Shader.h"

#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        void UpdateLight(DirectionalLight& sun, Shader& shader)
        {
            BindShader(shader);
            SetShaderUniform(shader, "sun.intensity", &sun.intensity, SHADER_UNIFORM_FLOAT);
            SetShaderUniform(shader, "sun.color", &sun.color, SHADER_UNIFORM_VEC3);
            SetShaderUniform(shader, "sun.direction", &sun.direction, SHADER_UNIFORM_VEC3);
            UnbindShader();
        }

        void UpdateLight(PointLight& pointLight, Shader& shader)
        {
            BindShader(shader);
            UnbindShader();
        }

        void UpdateLight(SpotLight& spotLight, Shader& shader)
        {
            const float cutoff = glm::cos(glm::radians(spotLight.innerRadius));
            const float outerCutoff = glm::cos(glm::radians(spotLight.outerRadius));

            BindShader(shader);
            SetShaderUniform(shader, "spotlight.intensity", &spotLight.intensity, SHADER_UNIFORM_FLOAT);
            SetShaderUniform(shader, "spotlight.cutoff", (void*)&cutoff, SHADER_UNIFORM_FLOAT);
            SetShaderUniform(shader, "spotlight.outerCutoff", (void*)&outerCutoff, SHADER_UNIFORM_FLOAT);
            SetShaderUniform(shader, "spotlight.position", &spotLight.position, SHADER_UNIFORM_VEC3);
            SetShaderUniform(shader, "spotlight.target", &spotLight.target, SHADER_UNIFORM_VEC3);
            SetShaderUniform(shader, "spotlight.color", &spotLight.color, SHADER_UNIFORM_VEC3);
            UnbindShader();
        }
    }
}
