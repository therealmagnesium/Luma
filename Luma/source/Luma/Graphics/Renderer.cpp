#include "Luma/Graphics/Renderer.h"
#include "Luma/Graphics/RenderCommand.h"
#include "Luma/Graphics/Mesh.h"

#include "Luma/Core/Application.h"
#include "Luma/Core/Log.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace Luma
{
    namespace Graphics
    {
        static RenderState state;
        static bool isInitialized = false;

        void InitializeMeshes();
        void InitializeShaders();

        void RendererInit()
        {
            if (isInitialized)
            {
                WARN("Cannot initialize the renderer  more than once");
                return;
            }

            const Core::ApplicationConfig& appInfo = Core::GetApplicationInfo();

            if (!SDL_Init(SDL_INIT_VIDEO))
            {
                FATAL("Failed to initialize SDL!");
                Core::QuitApplication();
            }

            state.window = CreateWindow(appInfo.windowWidth, appInfo.windowHeight, appInfo.name.c_str());

            InitializeMeshes();
            InitializeShaders();

            glLineWidth(5.f);

            INFO("The renderer was initialized successfully");
        }

        void RendererShutdown()
        {
            INFO("Shutting down the renderer...");

            for (u32 i = 0; i < RENDERER_MESH_COUNT; i++)
                DestroyMesh(state.meshes[i]);

            for (u32 i = 0; i < RENDERER_SHADER_COUNT; i++)
                UnloadShader(state.shaders[i]);
        }

        void RendererBegin()
        {
            if (state.primaryCamera != NULL)
            {
                Core::ApplicationConfig& appInfo = Core::GetApplicationInfo();
                float aspectRatio = appInfo.windowWidth / (float)appInfo.windowHeight;
                state.projection = glm::perspective(state.primaryCamera->fov, aspectRatio, 0.1f, 200.f);
            }

            BindShader(state.shaders[RENDERER_SHADER_PHONG]);
            SetShaderUniform(state.shaders[RENDERER_SHADER_PHONG], "viewMatrix", &GetPrimaryCamera()->view, SHADER_UNIFORM_MAT4);
            SetShaderUniform(state.shaders[RENDERER_SHADER_PHONG], "projectionMatrix", (void*)&GetProjection(), SHADER_UNIFORM_MAT4);
            SetShaderUniform(state.shaders[RENDERER_SHADER_PHONG], "viewWorldPosition", &GetPrimaryCamera()->position,
                             SHADER_UNIFORM_VEC3);
            UnbindShader();

            BindShader(state.shaders[RENDERER_SHADER_DEFAULT]);
            SetShaderUniform(state.shaders[RENDERER_SHADER_DEFAULT], "viewMatrix", &GetPrimaryCamera()->view, SHADER_UNIFORM_MAT4);
            SetShaderUniform(state.shaders[RENDERER_SHADER_DEFAULT], "projectionMatrix", (void*)&GetProjection(), SHADER_UNIFORM_MAT4);
            UnbindShader();

            state.primaryShader = &state.shaders[RENDERER_SHADER_DEFAULT];
        }

        void RendererEnd()
        {
            DisplayWindow(state.window);
            UnbindShader();
        }

        void RendererClear(float r, float g, float b)
        {
            RenderCommand::Clear(r, g, b);
        }

        void RendererDrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Color& color)
        {
            glm::mat4 transform = glm::mat4(1.f);
            transform = glm::translate(transform, position);
            transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
            transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
            transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
            transform = glm::scale(transform, scale);

            Material material = LoadMaterialDefault();
            material.albedo = color;
            material.shader = state.primaryShader;

            RendererDrawMesh(state.meshes[RENDERER_MESH_CUBE], transform, material);
        }

        void RendererDrawCubeWires(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Color& color)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            RendererDrawCube(position, rotation, scale, color);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        void RendererDrawMesh(Mesh& mesh, const glm::mat4& transform, Material& material)
        {
            Core::ApplicationConfig& appInfo = Core::GetApplicationInfo();
            const u32 albedoTextureSlot = 0;

            if (material.shader != NULL && GetPrimaryCamera() != NULL)
            {
                glm::mat4 normalMatrix = glm::transpose(glm::inverse(transform));

                BindShader(*material.shader);

                SetShaderUniform(*material.shader, "modelMatrix", (void*)&transform, SHADER_UNIFORM_MAT4);
                SetShaderUniform(*material.shader, "normalMatrix", &normalMatrix, SHADER_UNIFORM_MAT4);
                SetShaderUniform(*material.shader, "albedo", &material.albedo, SHADER_UNIFORM_VEC3);
                SetShaderUniform(*material.shader, "albedoTexture", (void*)&albedoTextureSlot, SHADER_UNIFORM_INT);

                BindTexture((Texture){.id = 0}, 0);

                if (material.albedoTexture != NULL)
                    BindTexture(*material.albedoTexture, 0);

                BindVertexArray(mesh.vertexArray);
                BindIndexBuffer(mesh.indexBuffer);

                RenderCommand::DrawIndexed(mesh.indices.size());

                UnbindIndexBuffer();
                UnbindVertexArray();

                UnbindTexture();
                UnbindShader();
            }
        }

        Shader& GetShaderDefault()
        {
            return state.shaders[RENDERER_SHADER_DEFAULT];
        }

        Shader& GetShaderUV()
        {
            return state.shaders[RENDERER_SHADER_UV];
        }

        Shader& GetShaderNormal()
        {
            return state.shaders[RENDERER_SHADER_NORMAL];
        }

        Shader& GetShaderPhong()
        {
            return state.shaders[RENDERER_SHADER_PHONG];
        }

        Shader& GetShaderPostProcessing()
        {
            return state.shaders[RENDERER_SHADER_POST_PROCESSING];
        }

        Mesh& GetMeshCube()
        {
            return state.meshes[RENDERER_MESH_CUBE];
        }

        Window& GetMainWindow()
        {
            return state.window;
        }

        Camera* GetPrimaryCamera()
        {
            return state.primaryCamera;
        }

        const glm::vec3& GetClearColor()
        {
            return state.clearColor;
        }

        const glm::mat4& GetProjection()
        {
            return state.projection;
        }

        void SetClearColor(float r, float g, float b)
        {
            state.clearColor.x = r;
            state.clearColor.y = g;
            state.clearColor.z = b;
        }

        void SetPrimaryCamera(Camera& camera)
        {
            state.primaryCamera = &camera;
        }

        void InitializeMeshes()
        {
            state.meshes[RENDERER_MESH_QUAD] = GenMeshQuad();
            state.meshes[RENDERER_MESH_CUBE] = GenMeshCube();
            state.meshes[RENDERER_MESH_SPHERE] = (Mesh){.vertexArray = 0};
        }

        void InitializeShaders()
        {
            state.shaders[RENDERER_SHADER_DEFAULT] = LoadShader("assets/shaders/Default_vs.glsl", "assets/shaders/Default_fs.glsl");
            CreateShaderUniformMVP(state.shaders[RENDERER_SHADER_DEFAULT]);
            CreateShaderUniform(state.shaders[RENDERER_SHADER_DEFAULT], "albedo");
            CreateShaderUniform(state.shaders[RENDERER_SHADER_DEFAULT], "albedoTexture");

            state.shaders[RENDERER_SHADER_UV] = LoadShader("assets/shaders/UV_vs.glsl", "assets/shaders/UV_fs.glsl");
            CreateShaderUniformMVP(state.shaders[RENDERER_SHADER_UV]);

            state.shaders[RENDERER_SHADER_NORMAL] = LoadShader("assets/shaders/Normal_vs.glsl", "assets/shaders/Normal_fs.glsl");
            CreateShaderUniformMVP(state.shaders[RENDERER_SHADER_NORMAL]);
            CreateShaderUniform(state.shaders[RENDERER_SHADER_NORMAL], "normalMatrix");

            state.shaders[RENDERER_SHADER_PHONG] = LoadShader("assets/shaders/Phong_vs.glsl", "assets/shaders/Phong_fs.glsl");
            CreateShaderUniformMVP(state.shaders[RENDERER_SHADER_PHONG]);
            CreateShaderUniform(state.shaders[RENDERER_SHADER_PHONG], "normalMatrix");
            CreateShaderUniform(state.shaders[RENDERER_SHADER_PHONG], "viewWorldPosition");
            CreateShaderUniform(state.shaders[RENDERER_SHADER_PHONG], "albedo");
            CreateShaderUniform(state.shaders[RENDERER_SHADER_PHONG], "albedoTexture");
            CreateShaderUniformLight(state.shaders[RENDERER_SHADER_PHONG], SHADER_UNIFORM_DIRECTIONAL);
            CreateShaderUniformLight(state.shaders[RENDERER_SHADER_PHONG], SHADER_UNIFORM_SPOTLIGHT);

            state.shaders[RENDERER_SHADER_POST_PROCESSING] = LoadShader("assets/shaders/Framebuffer_vs.glsl",
                                                                        "assets/shaders/Framebuffer_fs.glsl");
            CreateShaderUniform(state.shaders[RENDERER_SHADER_POST_PROCESSING], "projectionMatrix");
            CreateShaderUniform(state.shaders[RENDERER_SHADER_POST_PROCESSING], "albedoTexture");
        }
    }

}
