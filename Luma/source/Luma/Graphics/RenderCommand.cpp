#include "Luma/Graphics/RenderCommand.h"

#include <glad/glad.h>

namespace Luma
{
    namespace Graphics
    {
        namespace RenderCommand
        {
            void Clear(float r, float g, float b)
            {
                glClearColor(r, g, b, 1.f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }
        }
    }
}
