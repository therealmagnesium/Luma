#include "Luma/Core/Application.h"
#include "Luma/Core/Log.h"
#include "Luma/Core/Time.h"

#include "Luma/Graphics/RenderCommand.h"
#include "Luma/Graphics/Renderer.h"
#include "Luma/Graphics/Window.h"

#include <time.h>

namespace Luma
{
    namespace Core
    {
        static bool isInitialized = false;
        static ApplicationState state;

        void SetupApplication(Application* app)
        {
            if (isInitialized)
            {
                WARN("%s", "Cannot create the application more than once");
                return;
            }

            printf("============================================ Core Program Begins "
                   "=============================================\n");

            srand(time(NULL));
            state.isRunning = true;
            state.handle = app;

            TimeStateInitialize(60);
            Graphics::RendererInit();

            isInitialized = true;
            INFO("%s", "The application was initialized successfully");
            printf("============================================ Client Program Begins "
                   "=============================================\n");
        }

        void RunApplication()
        {
            state.handle->OnCreate();

            while (state.isRunning)
            {
                const glm::vec3& clearColor = Graphics::GetClearColor();
                Graphics::Window& mainWindow = Graphics::GetMainWindow();

                TimeStateUpdate();
                Graphics::HandleWindowEvents(mainWindow);

                state.handle->OnUpdate();

                Graphics::RendererBegin();

                Graphics::RenderCommand::Clear(V3_OPEN(clearColor));
                state.handle->OnRender();

                Graphics::RendererEnd();
            }

            printf("============================================ Client Program Ends "
                   "=============================================\n");
            state.handle->OnShutdown();

            printf("============================================ Core Program Ends "
                   "=============================================\n");
            Graphics::RendererShutdown();
        }

        void QuitApplication()
        {
            INFO("%s", "Quitting the application...");
            state.isRunning = false;
        }

        ApplicationConfig& GetApplicationInfo()
        {
            return state.handle->config;
        }
    }
}
