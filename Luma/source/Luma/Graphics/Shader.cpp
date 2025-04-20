#include "Luma/Graphics/Shader.h"
#include "Luma/Core/Base.h"
#include "Luma/Core/Log.h"
#include "Luma/Core/IO.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Luma
{
    namespace Graphics
    {
        static u32 CompileShader(u32 target, const char* path)
        {
            s32 success;
            char log[512];

            Core::File file = Core::ReadFile(path);
            if (!file.isValid)
            {
                WARN("Failed to compile shader %s!", path);
                return 0;
            }

            u32 targetId = glCreateShader(target);
            glShaderSource(targetId, 1, (const char* const*)&file.data, NULL);
            glCompileShader(targetId);
            glGetShaderiv(targetId, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(targetId, 512, NULL, log);
                ERROR("Failed to compile shader!\n%s", log);
            }

            delete file.data;
            file.data = NULL;

            return targetId;
        }

        Shader LoadShader(const char* vertexPath, const char* fragmentPath)
        {
            Shader shader;
            shader.id = glCreateProgram();
            shader.source.vertexPath = vertexPath;
            shader.source.fragmentPath = fragmentPath;

            u32 vertex = CompileShader(GL_VERTEX_SHADER, vertexPath);
            u32 fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentPath);

            int success;
            char log[512];

            glAttachShader(shader.id, vertex);
            glAttachShader(shader.id, fragment);
            glLinkProgram(shader.id);
            glGetProgramiv(shader.id, GL_LINK_STATUS, &success);

            if (!success)
            {
                glGetProgramInfoLog(shader.id, 512, NULL, log);
                ERROR("Failed to link shader!\n%s", log);
                return (Shader){};
            }

            INFO("Shader [%s, %s] loaded successfully with id %d", vertexPath, fragmentPath, shader.id);
            return shader;
        }

        void UnloadShader(Shader& shader)
        {
            INFO("Unloading shader [%s, %s]...",     //
                 shader.source.vertexPath.c_str(),   //
                 shader.source.fragmentPath.c_str(), //
                 shader.id);

            UnbindShader();
            glDeleteProgram(shader.id);
        }

        void BindShader(Shader& shader)
        {
            glUseProgram(shader.id);
        }

        void UnbindShader()
        {
            glUseProgram(0);
        }

        void Shader::CreateUniform(const char* name)
        {
            if (uniforms.find(name) != uniforms.end())
            {
                WARN("Uniform %s has already been created for shader with id %d", name, id);
                return;
            }
            uniforms[name] = glGetUniformLocation(id, name);

            if (uniforms[name] == -1)
                WARN("Shader with id %d could not find uniform \"%s\"", id, name);
        }

        void Shader::SetInt(const char* name, s32 value)
        {
            if (uniforms.find(name) != uniforms.end())
                glUniform1i(uniforms[name], value);
        }

        void Shader::SetFloat(const char* name, float value)
        {
            if (uniforms.find(name) != uniforms.end())
                glUniform1f(uniforms[name], value);
        }

        void Shader::SetVec3(const char* name, const glm::vec3& value)
        {
            if (uniforms.find(name) != uniforms.end())
                glUniform3fv(uniforms[name], 1, glm::value_ptr(value));
        }

        void Shader::SetVec4(const char* name, const glm::vec4& value)
        {
            if (uniforms.find(name) != uniforms.end())
                glUniform4fv(uniforms[name], 1, glm::value_ptr(value));
        }

        void Shader::SetMat4(const char* name, const glm::mat4& value)
        {
            if (uniforms.find(name) != uniforms.end())
                glUniformMatrix4fv(uniforms[name], 1, false, glm::value_ptr(value));
        }

    }
}
