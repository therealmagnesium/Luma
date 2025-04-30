#include "Luma/Graphics/Mesh.h"
#include "Luma/Core/Log.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

                (Vertex){glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.f, 0.f, 0.f)},  // D 8
                (Vertex){glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.f, 0.f, 0.f)}, // A 9
                (Vertex){glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.f, 0.f, 0.f)},  // E 10
                (Vertex){glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.f, 0.f, 0.f)},   // H 11
                (Vertex){glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f)},   // B 12
                (Vertex){glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f)},    // C 13
                (Vertex){glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(1.f, 0.f, 0.f)},     // G 14
                (Vertex){glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.f, 0.f, 0.f)},    // F 15

                (Vertex){glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.f, -1.f, 0.f)}, // A 16
                (Vertex){glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.f, -1.f, 0.f)},  // B 17
                (Vertex){glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.f, -1.f, 0.f)},   // F 18
                (Vertex){glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.f, -1.f, 0.f)},  // E 19
                (Vertex){glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f)},    // C 20
                (Vertex){glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f)},   // D 21
                (Vertex){glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.f, 1.f, 0.f)},    // H 22
                (Vertex){glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.f, 1.f, 0.f)},     // G 23
            };

            u32 indices[] = {0, 3, 2, 2, 1, 0, 4, 5, 6, 6, 7, 4,              // front and back
                             11, 8, 9, 9, 10, 11, 12, 13, 14, 14, 15, 12,     // left and right
                             16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20}; // bottom and top

            Mesh mesh = CreateMesh(vertices, LEN(vertices), indices, LEN(indices));
            return mesh;
        }

        Mesh GenMeshSphere(u32 stackCount, u32 sectorCount, float radius)
        {
            std::vector<Vertex> vertices;
            std::vector<u32> indices;

            vertices.reserve(stackCount * sectorCount);
            indices.reserve(stackCount * sectorCount);

            glm::vec3 position;
            glm::vec2 uvCoord;
            glm::vec3 normal;

            float xy;
            float lengthInv = 1.0f / radius; // vertex normal

            float sectorStep = 2 * M_PI / sectorCount;
            float stackStep = M_PI / stackCount;
            float sectorAngle, stackAngle;

            for (int i = 0; i <= stackCount; ++i)
            {
                stackAngle = M_PI / 2 - i * stackStep;  // starting from pi/2 to -pi/2
                xy = radius * cosf(stackAngle);         // r * cos(u)
                position.z = radius * sinf(stackAngle); // r * sin(u)

                // add (sectorCount+1) vertices per stack
                // first and last vertices have same position and normal, but different tex coords
                for (int j = 0; j <= sectorCount; ++j)
                {
                    Vertex vertex;
                    sectorAngle = j * sectorStep; // starting from 0 to 2pi

                    // vertex position (x, y, z)
                    position.x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
                    position.y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
                    vertex.position = position;

                    // normalized vertex normal (nx, ny, nz)
                    normal.x = position.x * lengthInv;
                    normal.y = position.y * lengthInv;
                    normal.z = position.z * lengthInv;
                    vertex.normal = normal;

                    // vertex tex coord (s, t) range between [0, 1]
                    uvCoord.x = (float)j / sectorCount;
                    uvCoord.y = (float)i / stackCount;
                    vertex.texCoord = uvCoord;

                    vertices.push_back(vertex);
                }
            }

            u32 k1, k2;
            for (int i = 0; i < stackCount; ++i)
            {
                k1 = i * (sectorCount + 1); // beginning of current stack
                k2 = k1 + sectorCount + 1;  // beginning of next stack

                for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
                {
                    // 2 triangles per sector excluding first and last stacks
                    // k1 => k2 => k1+1
                    if (i != 0)
                    {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    }

                    // k1+1 => k2 => k2+1
                    if (i != (stackCount - 1))
                    {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }

            Mesh mesh = CreateMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
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

        glm::mat4 GetMeshTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        {
            glm::mat4 transform = glm::mat4(1.f);
            transform = glm::translate(transform, position);
            transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
            transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
            transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
            transform = glm::scale(transform, scale);

            return transform;
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
