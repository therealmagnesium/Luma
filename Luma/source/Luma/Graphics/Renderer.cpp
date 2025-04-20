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

            INFO("The renderer was initialized successfully");
        }

        void RendererShutdown()
        {
            INFO("Shutting down the renderer...");

            UnloadShader(state.defaultShader);
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

                SetShaderUniform(state.defaultShader, "viewMatrix", &state.primaryCamera->view, SHADER_UNIFORM_MAT4);
                SetShaderUniform(state.defaultShader, "projectionMatrix", &state.projection, SHADER_UNIFORM_MAT4);
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

        Window& GetMainWindow()
        {
            return state.window;
        }

        const glm::vec3& GetClearColor()
        {
            return state.clearColor;
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
