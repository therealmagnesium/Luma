#include "Luma/Graphics/Renderer.h"
#include "Luma/Graphics/RenderCommand.h"

#include "Luma/Core/Application.h"
#include "Luma/Core/Log.h"

#include <SDL3/SDL.h>
#include <glm/ext/matrix_clip_space.hpp>

namespace Luma
{
    namespace Graphics
    {
        static RenderState state;
        static bool isInitialized = false;

        void InitializeStartupShaders();

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
            InitializeStartupShaders();

            INFO("The renderer was initialized successfully");
        }

        void RendererShutdown()
        {
            INFO("Shutting down the renderer...");

            UnloadShader(state.defaultShader);
            UnloadShader(state.uvShader);
            UnloadShader(state.normalShader);
            UnloadShader(state.phongShader);
            UnloadShader(state.shadowMapShader);
            UnloadShader(state.framebufferShader);
        }

        void RendererBegin()
        {
            if (state.primaryCamera != NULL)
            {
                Core::ApplicationConfig& appInfo = Core::GetApplicationInfo();
                float aspectRatio = appInfo.windowWidth / (float)appInfo.windowHeight;
                state.projection = glm::perspective(state.primaryCamera->fov, aspectRatio, 0.1f, 200.f);
            }

            BindShader(state.phongShader);
            SetShaderUniform(state.phongShader, "viewMatrix", &GetPrimaryCamera()->view, SHADER_UNIFORM_MAT4);
            SetShaderUniform(state.phongShader, "projectionMatrix", (void*)&GetProjection(), SHADER_UNIFORM_MAT4);
            SetShaderUniform(state.phongShader, "viewWorldPosition", &GetPrimaryCamera()->position,
                             SHADER_UNIFORM_VEC3);
            UnbindShader();

            BindShader(state.defaultShader);
            SetShaderUniform(state.defaultShader, "viewMatrix", &GetPrimaryCamera()->view, SHADER_UNIFORM_MAT4);
            SetShaderUniform(state.defaultShader, "projectionMatrix", (void*)&GetProjection(), SHADER_UNIFORM_MAT4);
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

        Shader& GetDefaultShader()
        {
            return state.defaultShader;
        }

        Shader& GetUVShader()
        {
            return state.uvShader;
        }

        Shader& GetNormalShader()
        {
            return state.normalShader;
        }

        Shader& GetPhongShader()
        {
            return state.phongShader;
        }

        Shader& GetShadowMapShader()
        {
            return state.shadowMapShader;
        }

        Shader& GetFramebufferShader()
        {
            return state.framebufferShader;
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

        void InitializeStartupShaders()
        {
            state.defaultShader = LoadShader("assets/shaders/Default_vs.glsl", "assets/shaders/Default_fs.glsl");
            CreateShaderUniform(state.defaultShader, "modelMatrix");
            CreateShaderUniform(state.defaultShader, "viewMatrix");
            CreateShaderUniform(state.defaultShader, "projectionMatrix");
            CreateShaderUniform(state.defaultShader, "albedo");
            CreateShaderUniform(state.defaultShader, "albedoTexture");

            state.uvShader = LoadShader("assets/shaders/UV_vs.glsl", "assets/shaders/UV_fs.glsl");
            CreateShaderUniform(state.uvShader, "modelMatrix");
            CreateShaderUniform(state.uvShader, "viewMatrix");
            CreateShaderUniform(state.uvShader, "projectionMatrix");

            state.normalShader = LoadShader("assets/shaders/Normal_vs.glsl", "assets/shaders/Normal_fs.glsl");
            CreateShaderUniform(state.normalShader, "modelMatrix");
            CreateShaderUniform(state.normalShader, "viewMatrix");
            CreateShaderUniform(state.normalShader, "projectionMatrix");
            CreateShaderUniform(state.normalShader, "normalMatrix");

            state.phongShader = LoadShader("assets/shaders/Phong_vs.glsl", "assets/shaders/Phong_fs.glsl");
            CreateShaderUniform(state.phongShader, "modelMatrix");
            CreateShaderUniform(state.phongShader, "viewMatrix");
            CreateShaderUniform(state.phongShader, "projectionMatrix");
            CreateShaderUniform(state.phongShader, "normalMatrix");
            CreateShaderUniform(state.phongShader, "viewWorldPosition");
            CreateShaderUniform(state.phongShader, "albedo");
            CreateShaderUniform(state.phongShader, "albedoTexture");
            CreateShaderUniform(state.phongShader, "sun.intensity");
            CreateShaderUniform(state.phongShader, "sun.color");
            CreateShaderUniform(state.phongShader, "sun.direction");
            CreateShaderUniform(state.phongShader, "spotlight.intensity");
            CreateShaderUniform(state.phongShader, "spotlight.cutoff");
            CreateShaderUniform(state.phongShader, "spotlight.outerCutoff");
            CreateShaderUniform(state.phongShader, "spotlight.position");
            CreateShaderUniform(state.phongShader, "spotlight.target");
            CreateShaderUniform(state.phongShader, "spotlight.color");

            state.shadowMapShader = LoadShader("assets/shaders/ShadowMap_vs.glsl", "assets/shaders/ShadowMap_fs.glsl");
            CreateShaderUniform(state.shadowMapShader, "modelMatrix");
            CreateShaderUniform(state.shadowMapShader, "lightSpaceMatrix");

            state.framebufferShader =
                LoadShader("assets/shaders/Framebuffer_vs.glsl", "assets/shaders/Framebuffer_fs.glsl");
            CreateShaderUniform(state.framebufferShader, "projectionMatrix");
            CreateShaderUniform(state.framebufferShader, "albedoTexture");
        }
    }

}
