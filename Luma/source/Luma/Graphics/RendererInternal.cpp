#include "Luma/Graphics/RendererInternal.h"
#include "Luma/Core/Base.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Luma
{
    namespace Graphics
    {
        u32 CreateVertexArray()
        {
            u32 vao = 0;
            glGenVertexArrays(1, &vao);

            return vao;
        }

        u32 CreateVertexBuffer()
        {
            u32 vbo = 0;
            glGenBuffers(1, &vbo);

            return vbo;
        }

        void DestroyVertexArray(u32& vao)
        {
            glDeleteVertexArrays(1, &vao);
        }

        void DestroyVertexBuffer(u32& vbo)
        {
            glDeleteBuffers(1, &vbo);
        }

        void BindVertexArray(u32& vao)
        {
            glBindVertexArray(vao);
        }

        void BindVertexBuffer(u32& vbo)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
        }

        void SetVertexBufferData(void* data, u32 bufferSize)
        {
            glBufferData(GL_ARRAY_BUFFER, bufferSize, data, GL_STATIC_DRAW);
        }

        void SetVertexArrayAttribute(u32 location, u32 elementCount, u32 stride, u32 offset)
        {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, elementCount, GL_FLOAT, false, stride, (void*)(u64)offset);
        }

        void UnbindVertexArray()
        {
            glBindVertexArray(0);
        }

        void UnbindVertexBuffer()
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}
