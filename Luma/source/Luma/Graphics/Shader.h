#pragma once
#include "Luma/Core/Base.h"

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Luma
{
    namespace Graphics
    {
        enum ShaderUniformType
        {
            SHADER_UNIFORM_INT = 0,
            SHADER_UNIFORM_FLOAT,
            SHADER_UNIFORM_VEC3,
            SHADER_UNIFORM_VEC4,
            SHADER_UNIFORM_MAT4,
        };

        enum ShaderUniformLightType
        {
            SHADER_UNIFORM_DIRECTIONAL = 0,
            SHADER_UNIFORM_SPOTLIGHT
        };

        struct ShaderSource
        {
            std::string vertexPath;
            std::string fragmentPath;
        };

        struct Shader
        {
            u32 id = 0;
            std::unordered_map<std::string, s32> uniforms;
            ShaderSource source;
        };

        Shader LoadShader(const char* vertexPath, const char* fragmentPath);
        void UnloadShader(Shader& shader);

        void BindShader(Shader& shader);
        void UnbindShader();

        void CreateShaderUniform(Shader& shader, const char* name);
        void CreateShaderUniformMVP(Shader& shader);
        void CreateShaderUniformMaterial(Shader& shader);
        void CreateShaderUniformLight(Shader& shader, ShaderUniformLightType type);
        void SetShaderUniform(Shader& shader, const char* name, void* data, ShaderUniformType uniformType);
    }
}
