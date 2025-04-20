#pragma once
#include "Luma/Graphics/Camera.h"
#include "Luma/Graphics/Shader.h"
#include "Luma/Graphics/Window.h"

#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        struct RenderState
        {
            Shader defaultShader;
            Window window;
            Camera* primaryCamera = NULL;
            glm::vec3 clearColor = glm::vec3(1.f);
            glm::mat4 projection = glm::mat4(1.f);
        };

        void RendererInit();
        void RendererShutdown();
        void RendererBegin();
        void RendererEnd();
        void RendererClear(float r, float g, float b);

        Shader& GetDefaultShader();
        Window& GetMainWindow();
        const glm::vec3& GetClearColor();

        void SetClearColor(float r, float g, float b);
        void SetPrimaryCamera(Camera& camera);
    }
}
