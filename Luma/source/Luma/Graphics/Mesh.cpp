#include "Luma/Graphics/Mesh.h"
#include "Luma/Graphics/RenderCommand.h"
#include "Luma/Graphics/Renderer.h"
#include "Luma/Graphics/Shader.h"

#include "Luma/Core/Application.h"
#include "Luma/Core/Log.h"

#include <glad/glad.h>

namespace Luma
{
    namespace Graphics
    {
        void SubmitMesh(Mesh& mesh);

        Mesh CreateMesh(Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount)
        {
            Mesh mesh;
            mesh.vertexArray = CreateVertexArray();
            mesh.vertexBuffer = CreateBuffer();
            mesh.indexBuffer = CreateBuffer();

            mesh.vertices.insert(mesh.vertices.end(), &vertices[0], &vertices[vertexCount]);
            mesh.indices.insert(mesh.indices.end(), &indices[0], &indices[indexCount]);

            SubmitMesh(mesh);

            INFO("Mesh created successfully with an ID of %d", mesh.vertexArray);
            return mesh;
        }

        Mesh GenMeshQuad()
        {
            Vertex vertices[] = {
                (Vertex){glm::vec3(-1.f, -1.f, 0.0f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f)}, // bottom left
                (Vertex){glm::vec3(1.f, -1.f, 0.0f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)},  // bottom right
                (Vertex){glm::vec3(1.f, 1.f, 0.0f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f)},   // top right
                (Vertex){glm::vec3(-1.f, 1.f, 0.0f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f)}   // top left
            };

            u32 indices[] = {
                0, 1, 2, // first Triangle
                2, 3, 0  // second Triangle
            };

            Mesh mesh = CreateMesh(vertices, LEN(vertices), indices, LEN(indices));
            return mesh;
        }

        Mesh GenMeshCube()
        {
            Vertex vertices[] = {
                (Vertex){glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.f, 0.f, -1.f)}, // A 0
                (Vertex){glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.f, 0.f, -1.f)},  // B 1
                (Vertex){glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.f, 0.f, -1.f)},   // C 2
                (Vertex){glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.f, 0.f, -1.f)},  // D 3
                (Vertex){glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.f, 0.f, 1.f)},   // E 4
                (Vertex){glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.f, 0.f, 1.f)},    // F 5
                (Vertex){glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.f, 0.f, 1.f)},     // G 6
                (Vertex){glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.f, 0.f, 1.f)},    // H 7

                (Vertex){glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f)},  // D 8
                (Vertex){glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f)}, // A 9
                (Vertex){glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(1.f, 0.f, 0.f)},  // E 10
                (Vertex){glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.f, 0.f, 0.f)},   // H 11
                (Vertex){glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.f, 0.f, 0.f)}, // B 12
                (Vertex){glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.f, 0.f, 0.f)},  // C 13
                (Vertex){glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.f, 0.f, 0.f)},   // G 14
                (Vertex){glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.f, 0.f, 0.f)},  // F 15

                (Vertex){glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.f, -1.f, 0.f)}, // A 16
                (Vertex){glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.f, -1.f, 0.f)},  // B 17
                (Vertex){glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.f, -1.f, 0.f)},   // F 18
                (Vertex){glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.f, -1.f, 0.f)},  // E 19
                (Vertex){glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f)},    // C 20
                (Vertex){glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f)},   // D 21
                (Vertex){glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.f, 1.f, 0.f)},    // H 22
                (Vertex){glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.f, 1.f, 0.f)},     // G 23
            };

            u32 indices[] = {0,  3,  2,  2,  1,  0,  4,  5,  6,  6,  7,  4,   // front and back
                             11, 8,  9,  9,  10, 11, 12, 13, 14, 14, 15, 12,  // left and right
                             16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20}; // bottom and top

            Mesh mesh = CreateMesh(vertices, LEN(vertices), indices, LEN(indices));
            return mesh;
        }

        void DestroyMesh(Mesh& mesh)
        {
            if (mesh.vertexArray != 0)
            {
                INFO("Destorying mesh with an ID of %d", mesh.vertexArray);
                DestroyVertexArray(mesh.vertexArray);
                DestroyBuffer(mesh.vertexBuffer);
                DestroyBuffer(mesh.indexBuffer);

                mesh.vertexArray = 0;
                mesh.vertexBuffer = 0;
                mesh.indexBuffer = 0;
            }
        }

        void DrawMesh(Mesh& mesh, const glm::mat4& transform, Material& material)
        {
            Core::ApplicationConfig& appInfo = Core::GetApplicationInfo();
            const u32 albedoTextureSlot = 0;

            if (material.shader != NULL && GetPrimaryCamera() != NULL)
            {
                glm::mat4 normalMatrix = glm::transpose(glm::inverse(transform));

                BindShader(*material.shader);

                SetShaderUniform(*material.shader, "modelMatrix", (void*)&transform, SHADER_UNIFORM_MAT4);
                SetShaderUniform(*material.shader, "viewMatrix", &GetPrimaryCamera()->view, SHADER_UNIFORM_MAT4);
                SetShaderUniform(*material.shader, "projectionMatrix", (void*)&GetProjection(), SHADER_UNIFORM_MAT4);
                SetShaderUniform(*material.shader, "normalMatrix", &normalMatrix, SHADER_UNIFORM_MAT4);
                SetShaderUniform(*material.shader, "viewWorldPosition", &GetPrimaryCamera()->position,
                                 SHADER_UNIFORM_VEC3);
                SetShaderUniform(*material.shader, "albedo", &material.albedo, SHADER_UNIFORM_VEC3);
                SetShaderUniform(*material.shader, "albedoTexture", (void*)&albedoTextureSlot, SHADER_UNIFORM_INT);

                if (material.albedoTexture != NULL)
                    BindTexture(*material.albedoTexture);
                else
                    BindTexture((Texture){.id = 0});

                BindVertexArray(mesh.vertexArray);
                BindIndexBuffer(mesh.indexBuffer);

                RenderCommand::DrawIndexed(mesh.indices.size());

                UnbindIndexBuffer();
                UnbindVertexArray();

                UnbindTexture();
                UnbindShader();
            }
        }

        void SubmitMesh(Mesh& mesh)
        {
            const u32 vertexCount = mesh.vertices.size();
            const u32 indexCount = mesh.indices.size();
            Vertex* vertices = mesh.vertices.data();
            u32* indices = mesh.indices.data();

            BindVertexArray(mesh.vertexArray);
            BindVertexBuffer(mesh.vertexBuffer);
            BindIndexBuffer(mesh.indexBuffer);

            SetVertexBufferData(vertices, vertexCount * sizeof(Vertex));
            SetIndexBufferData(indices, indexCount * sizeof(u32));

            SetVertexArrayAttribute(0, 3, offsetof(Vertex, position));
            SetVertexArrayAttribute(1, 2, offsetof(Vertex, texCoord));
            SetVertexArrayAttribute(2, 3, offsetof(Vertex, normal));

            UnbindVertexArray();
            UnbindVertexBuffer();
            UnbindIndexBuffer();
        }
    }
}
