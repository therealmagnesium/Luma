#pragma once
#include "Luma/Core/Base.h"
#include "Luma/Graphics/Window.h"

#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        struct RenderState
        {
            Window window;
            glm::vec3 clearColor = glm::vec3(1.f);
        };

        void RendererInit();
        void RendererShutdown();
        void RendererBegin();
        void RendererEnd();
        void RendererClear(float r, float g, float b);

        Window& GetMainWindow();
        const glm::vec3& GetClearColor();

        void SetClearColor(float r, float g, float b);
    }
}
