#pragma once
#include "Luma/Core/Base.h"
#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        struct Vertex
        {
            glm::vec3 position;
            glm::vec2 texCoord;
            glm::vec3 normal;
        };

        u32 CreateVertexArray();
        u32 CreateVertexBuffer();
        void DestroyVertexArray(u32& vao);
        void DestroyVertexBuffer(u32& vbo);

        void BindVertexArray(u32& vao);
        void BindVertexBuffer(u32& vbo);

        void SetVertexBufferData(void* data, u32 bufferSize);
        void SetVertexArrayAttribute(u32 location, u32 elementCount, u32 stride, u32 offset);

        void UnbindVertexArray();
        void UnbindVertexBuffer();
    }
}
