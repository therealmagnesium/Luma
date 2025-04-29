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
            Shader uvShader;
            Shader normalShader;
            Shader phongShader;
            Shader framebufferShader;
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
        Shader& GetUVShader();
        Shader& GetNormalShader();
        Shader& GetPhongShader();
        Shader& GetFramebufferShader();
        Window& GetMainWindow();
        Camera* GetPrimaryCamera();
        const glm::vec3& GetClearColor();
        const glm::mat4& GetProjection();

        void SetClearColor(float r, float g, float b);
        void SetPrimaryCamera(Camera& camera);
    }
}
