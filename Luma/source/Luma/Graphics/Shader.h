#pragma once
#include "Luma/Core/Base.h"

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Luma
{
    namespace Graphics
    {
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

            void CreateUniform(const char* name);
            void SetInt(const char* name, s32 value);
            void SetFloat(const char* name, float value);
            void SetVec3(const char* name, const glm::vec3& value);
            void SetVec4(const char* name, const glm::vec4& value);
            void SetMat4(const char* name, const glm::mat4& value);
        };

        Shader LoadShader(const char* vertexPath, const char* fragmentPath);
        void UnloadShader(Shader& shader);

        void BindShader(Shader& shader);
        void UnbindShader();
    }
}
