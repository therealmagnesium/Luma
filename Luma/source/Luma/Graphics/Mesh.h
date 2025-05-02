#pragma once
#include "Luma/Core/Base.h"
#include "Luma/Graphics/RendererInternal.h"

#include <vector>
#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        struct Mesh
        {
            u32 vertexArray = 0;
            u32 vertexBuffer = 0;
            u32 indexBuffer = 0;
            std::vector<Vertex> vertices;
            std::vector<u32> indices;
        };

        Mesh CreateMesh(Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount);
        Mesh GenMeshQuad();
        Mesh GenMeshCube();
        Mesh GenMeshSphere(u32 stackCount, u32 sectorCount, float radius);
        void DestroyMesh(Mesh& mesh);

        glm::mat4 GetMeshTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    }
}
