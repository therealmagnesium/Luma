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

            INFO("Shader [%s, %s] loaded successfully with an ID of %d", vertexPath, fragmentPath, shader.id);
            return shader;
        }

        void UnloadShader(Shader& shader)
        {
            INFO("Unloading shader [%s, %s]...",   //
                 shader.source.vertexPath.c_str(), //
                 shader.source.fragmentPath.c_str());

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

        void CreateShaderUniform(Shader& shader, const char* name)
        {
            if (shader.uniforms.find(name) != shader.uniforms.end())
            {
                WARN("Uniform %s has already been created for shader with id %d", name, shader.id);
                return;
            }
            shader.uniforms[name] = glGetUniformLocation(shader.id, name);

            if (shader.uniforms[name] == -1)
                WARN("Shader with id %d could not find uniform \"%s\"", shader.id, name);
        }

        void SetShaderUniform(Shader& shader, const char* name, void* data, ShaderUniformType uniformType)
        {
            if (shader.uniforms.find(name) != shader.uniforms.end() && data != NULL)
            {
                switch (uniformType)
                {
                    case SHADER_UNIFORM_INT:
                    {
                        u32* newData = (u32*)data;
                        glUniform1i(shader.uniforms[name], *newData);
                        break;
                    }

                    case SHADER_UNIFORM_FLOAT:
                    {
                        float* newData = (float*)data;
                        glUniform1f(shader.uniforms[name], *newData);
                        break;
                    }

                    case SHADER_UNIFORM_VEC3:
                    {
                        glm::vec3* newData = (glm::vec3*)data;
                        glUniform3fv(shader.uniforms[name], 1, glm::value_ptr(*newData));
                        break;
                    }

                    case SHADER_UNIFORM_VEC4:
                    {
                        glm::vec4* newData = (glm::vec4*)data;
                        glUniform4fv(shader.uniforms[name], 1, glm::value_ptr(*newData));
                        break;
                    }

                    case SHADER_UNIFORM_MAT4:
                    {
                        glm::vec4* newData = (glm::vec4*)data;
                        glUniformMatrix4fv(shader.uniforms[name], 1, false, glm::value_ptr(*newData));
                        break;
                    }

                    default:
                        break;
                }
            }
        }

    }
}
