#include "Luma/Graphics/Lights.h"
#include "Luma/Graphics/Shader.h"

#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        void DrawLight(DirectionalLight& sun, Shader& shader)
        {
            BindShader(shader);
            SetShaderUniform(shader, "sun.intensity", &sun.intensity, SHADER_UNIFORM_FLOAT);
            SetShaderUniform(shader, "sun.color", &sun.color, SHADER_UNIFORM_VEC3);
            SetShaderUniform(shader, "sun.direction", &sun.direction, SHADER_UNIFORM_VEC3);
            UnbindShader();
        }

        void DrawLight(PointLight& pointLight, Shader& shader)
        {
            BindShader(shader);
            UnbindShader();
        }
    }
}
