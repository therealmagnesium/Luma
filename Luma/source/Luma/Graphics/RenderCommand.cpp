#include "Luma/Graphics/RenderCommand.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        namespace RenderCommand
        {
            void Clear(float r, float g, float b)
            {
                glClearColor(r, g, b, 1.f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            }

            void SetViewport(u32 width, u32 height)
            {
                glViewport(0, 0, width, height);
            }

            void WriteToDepth(bool enabled)
            {
                (enabled) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
            }

            void DrawArrays(u32 numIndices)
            {
                glDrawArrays(GL_TRIANGLES, 0, numIndices);
            }

            void DrawIndexed(u32 numIndices)
            {
                glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
            }
        }
    }
}
