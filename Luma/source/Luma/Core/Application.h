#pragma once
#include "Luma/Core/Base.h"
#include "Luma/Graphics/Window.h"

#include <glm/glm.hpp>
#include <string>

namespace Luma
{
    namespace Core
    {
        struct ApplicationConfig
        {
            std::string name = "Untitled";
            std::string author = "";
            u32 windowWidth = 1280;
            u32 windowHeight = 720;
        };

        struct Application
        {
            ApplicationConfig config;

            void (*OnCreate)() = NULL;
            void (*OnUpdate)() = NULL;
            void (*OnRender)() = NULL;
            void (*OnRenderUI)() = NULL;
            void (*OnShutdown)() = NULL;
        };

        struct ApplicationState
        {
            bool isRunning = false;
            Application* handle = NULL;
        };

        void SetupApplication(Application* app);
        void RunApplication();
        void QuitApplication();

        ApplicationConfig& GetApplicationInfo();
    }
}
