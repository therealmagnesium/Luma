#pragma once
#include "Luma/Core/Base.h"

namespace Luma
{
    namespace Graphics
    {
        namespace RenderCommand
        {
            void Clear(float r, float g, float b);
            void SetViewport(u32 width, u32 height);
            void DrawArrays(u32 numIndices);
        }
    }
}
