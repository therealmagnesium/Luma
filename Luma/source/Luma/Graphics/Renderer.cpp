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

        void RendererInit()
        {
            const Core::ApplicationConfig& appInfo = Core::GetApplicationInfo();

            if (!SDL_Init(SDL_INIT_VIDEO))
            {
                FATAL("%s", "Failed to initialize SDL!");
                Core::QuitApplication();
            }

            state.window = CreateWindow(appInfo.windowWidth, appInfo.windowHeight, appInfo.name.c_str());

            state.defaultShader = LoadShader("assets/shaders/Default_vs.glsl", "assets/shaders/Default_fs.glsl");
            CreateShaderUniform(state.defaultShader, "modelMatrix");
            CreateShaderUniform(state.defaultShader, "viewMatrix");
            CreateShaderUniform(state.defaultShader, "projectionMatrix");
            CreateShaderUniform(state.defaultShader, "material.albedo");
            CreateShaderUniform(state.defaultShader, "material.albedoTexture");

            state.uvShader = LoadShader("assets/shaders/UV_vs.glsl", "assets/shaders/UV_fs.glsl");
            CreateShaderUniform(state.uvShader, "modelMatrix");
            CreateShaderUniform(state.uvShader, "viewMatrix");
            CreateShaderUniform(state.uvShader, "projectionMatrix");

            state.normalShader = LoadShader("assets/shaders/Normal_vs.glsl", "assets/shaders/Normal_fs.glsl");
            CreateShaderUniform(state.normalShader, "modelMatrix");
            CreateShaderUniform(state.normalShader, "viewMatrix");
            CreateShaderUniform(state.normalShader, "projectionMatrix");

            INFO("The renderer was initialized successfully");
        }

        void RendererShutdown()
        {
            INFO("Shutting down the renderer...");

            UnloadShader(state.defaultShader);
            UnloadShader(state.uvShader);
            DestroyWindow(state.window);
        }

        void RendererBegin()
        {
            BindShader(state.defaultShader);

            if (state.primaryCamera != NULL)
            {
                Core::ApplicationConfig& appInfo = Core::GetApplicationInfo();
                float aspectRatio = appInfo.windowWidth / (float)appInfo.windowHeight;
                state.projection = glm::perspective(state.primaryCamera->fov, aspectRatio, 0.1f, 200.f);
            }
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
    }

}
