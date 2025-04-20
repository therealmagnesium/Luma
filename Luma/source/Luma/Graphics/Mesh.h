#pragma once
#include "Luma/Core/Base.h"
#include "Luma/Graphics/RendererInternal.h"

#include <vector>

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
        void DestroyMesh(Mesh& mesh);
        void DrawMesh(Mesh& mesh, const glm::mat4& transform);
    }
}
