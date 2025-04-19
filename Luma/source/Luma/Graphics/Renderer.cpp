#include "Luma/Graphics/Renderer.h"
#include "Luma/Graphics/RenderCommand.h"

#include "Luma/Core/Application.h"
#include "Luma/Core/Log.h"

namespace Luma
{
    namespace Graphics
    {
        static RenderState state;

        void RendererInit()
        {
            const Core::ApplicationConfig& appInfo = Core::GetApplicationInfo();
            state.window = CreateWindow(appInfo.windowWidth, appInfo.windowHeight, appInfo.name.c_str());

            INFO("The renderer was initialized successfully");
        }

        void RendererShutdown()
        {
            INFO("Shutting down the renderer...");
            DestroyWindow(state.window);
        }

        void RendererBegin()
        {
        }

        void RendererEnd()
        {
            DisplayWindow(state.window);
        }

        void RendererClear(float r, float g, float b)
        {
            RenderCommand::Clear(r, g, b);
        }

        Window& GetMainWindow()
        {
            return state.window;
        }

    }

}
