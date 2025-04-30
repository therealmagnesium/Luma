#pragma once
#include "Luma/Graphics/Camera.h"
#include "Luma/Graphics/Colors.h"
#include "Luma/Graphics/Mesh.h"
#include "Luma/Graphics/Shader.h"
#include "Luma/Graphics/Window.h"

#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        enum RenderStateShaderType
        {
            RENDERER_SHADER_DEFAULT = 0,
            RENDERER_SHADER_UV,
            RENDERER_SHADER_NORMAL,
            RENDERER_SHADER_PHONG,
            RENDERER_SHADER_POST_PROCESSING,
            RENDERER_SHADER_COUNT,
        };

        enum RenderStateMeshType
        {
            RENDERER_MESH_QUAD = 0,
            RENDERER_MESH_CUBE,
            RENDERER_MESH_SPHERE,
            RENDERER_MESH_COUNT,
        };

        struct RenderState
        {
            Window window;
            Mesh meshes[RENDERER_MESH_COUNT];
            Shader shaders[RENDERER_SHADER_COUNT];
            glm::vec3 clearColor = glm::vec3(1.f);
            glm::mat4 projection = glm::mat4(1.f);
            Camera* primaryCamera = NULL;
            Shader* primaryShader = NULL;
        };

        void RendererInit();
        void RendererShutdown();
        void RendererBegin();
        void RendererEnd();
        void RendererClear(float r, float g, float b);
        void RendererDrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Color& color);
        void RendererDrawCubeWires(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Color& color);
        void RendererDrawMesh(Mesh& mesh, const glm::mat4& transform, Material& material);

        Shader& GetShaderDefault();
        Shader& GetShaderUV();
        Shader& GetShaderNormal();
        Shader& GetShaderPhong();
        Shader& GetShaderPostProcessing();
        Mesh& GetMeshCube();
        Window& GetMainWindow();
        Camera* GetPrimaryCamera();
        const glm::vec3& GetClearColor();
        const glm::mat4& GetProjection();

        void SetClearColor(float r, float g, float b);
        void SetPrimaryCamera(Camera& camera);
    }
}
