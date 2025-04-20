#include "Luma/Graphics/Mesh.h"
#include "Luma/Graphics/RenderCommand.h"
#include "Luma/Graphics/Renderer.h"
#include "Luma/Graphics/Shader.h"

namespace Luma
{
    namespace Graphics
    {
        Mesh CreateMesh(Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount)
        {
            Mesh mesh;
            mesh.vertexArray = CreateVertexArray();
            mesh.vertexBuffer = CreateBuffer();
            mesh.indexBuffer = CreateBuffer();

            BindVertexArray(mesh.vertexArray);
            BindVertexBuffer(mesh.vertexBuffer);
            BindIndexBuffer(mesh.indexBuffer);

            SetVertexBufferData(vertices, vertexCount * sizeof(Vertex));
            SetIndexBufferData(indices, indexCount * sizeof(u32));

            SetVertexArrayAttribute(0, 3, offsetof(Vertex, position));
            SetVertexArrayAttribute(1, 2, offsetof(Vertex, texCoord));

            UnbindVertexArray();
            UnbindVertexBuffer();
            UnbindIndexBuffer();

            mesh.vertices.insert(mesh.vertices.end(), &vertices[0], &vertices[vertexCount]);
            mesh.indices.insert(mesh.indices.end(), &indices[0], &indices[indexCount]);

            return mesh;
        }

        void DestroyMesh(Mesh& mesh)
        {
            DestroyVertexArray(mesh.vertexArray);
            DestroyBuffer(mesh.vertexBuffer);
            DestroyBuffer(mesh.indexBuffer);
        }

        void DrawMesh(Mesh& mesh, const glm::mat4& transform, Material& material)
        {
            if (material.shader != NULL)
            {
                SetShaderUniform(*material.shader, "modelMatrix", (void*)&transform, SHADER_UNIFORM_MAT4);
                SetShaderUniform(*material.shader, "material.albedo", &material.albedo, SHADER_UNIFORM_VEC3);
                SetShaderUniform(*material.shader, "material.albedoTexture", 0, SHADER_UNIFORM_INT);

                if (material.albedoTexture != NULL)
                    BindTexture(*material.albedoTexture, 0);

                BindVertexArray(mesh.vertexArray);
                BindIndexBuffer(mesh.indexBuffer);

                RenderCommand::DrawIndexed(6);

                UnbindIndexBuffer();
                UnbindVertexArray();
            }
        }
    }
}
