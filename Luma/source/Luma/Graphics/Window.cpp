#include "Luma/Graphics/Window.h"
#include "Luma/Graphics/RenderCommand.h"

#include "Luma/Core/Application.h"
#include "Luma/Core/Input.h"
#include "Luma/Core/Log.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <glad/glad.h>

namespace Luma
{
    namespace Graphics
    {
        Window CreateWindow(u32 width, u32 height, const char* title)
        {
            Window window;
            window.width = width;
            window.height = height;
            window.title = title;

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
            SDL_GL_SetSwapInterval(1);

            window.handle = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
            ASSERT(window.handle != NULL, "Could not create the window!");

            window.context = SDL_GL_CreateContext(window.handle);
            ASSERT(window.context != NULL, "Could not create the window's OpenGL context!");
            SDL_GL_MakeCurrent(window.handle, window.context);

            gladLoadGL();

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_CULL_FACE);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            INFO("Window \"%s\" was successfully created with an OpenGL context", window.title.c_str());
            INFO("GPU vendor: %s", glGetString(GL_VENDOR));
            INFO("GPU specs: %s", glGetString(GL_RENDERER));
            INFO("OpenGL version: %s", glGetString(GL_VERSION));

            return window;
        }

        void HandleWindowEvents(Window& window)
        {
            SDL_Event event;

            for (int i = 0; i < Core::MOUSE_BUTTON_COUNT; i++)
                Core::Input.mouse.buttonsClicked[i] = false;

            for (int i = 0; i < Core::KEY_COUNT; i++)
                Core::Input.keyboard.keysPressed[i] = false;

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_EVENT_QUIT:
                        Core::QuitApplication();
                        break;

                    case SDL_EVENT_KEY_DOWN:
                        Core::Input.keyboard.keysPressed[event.key.scancode] =
                            !Core::Input.keyboard.keysHeld[event.key.scancode];
                        Core::Input.keyboard.keysHeld[event.key.scancode] = true;
                        break;

                    case SDL_EVENT_KEY_UP:
                        Core::Input.keyboard.keysPressed[event.key.scancode] = false;
                        Core::Input.keyboard.keysHeld[event.key.scancode] = false;
                        break;

                    case SDL_EVENT_MOUSE_MOTION:
                        Core::Input.mouse.position.x = event.motion.x;
                        Core::Input.mouse.position.y = event.motion.y;
                        Core::Input.mouse.relative.x = event.motion.xrel;
                        Core::Input.mouse.relative.y = event.motion.yrel;
                        break;

                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                        Core::Input.mouse.buttonsClicked[event.button.button] =
                            !Core::Input.mouse.buttonsHeld[event.button.button];
                        Core::Input.mouse.buttonsHeld[event.button.button] = true;
                        break;

                    case SDL_EVENT_MOUSE_BUTTON_UP:
                        Core::Input.mouse.buttonsClicked[event.button.button] = false;
                        Core::Input.mouse.buttonsHeld[event.button.button] = false;
                        break;

                    case SDL_EVENT_WINDOW_RESIZED:
                    {
                        Core::ApplicationConfig& config = Core::GetApplicationInfo();

                        window.width = event.window.data1;
                        window.height = event.window.data2;
                        config.windowWidth = window.width;
                        config.windowHeight = window.height;

                        RenderCommand::SetViewport(window.width, window.height);
                        //  INFO("Resized window \"%s\" to %dx%d", window.title.c_str(), window.width, window.height);
                    }
                }
            }
        }

        void DisplayWindow(Window& window)
        {
            SDL_GL_SwapWindow(window.handle);
        }

        void DestroyWindow(Window& window)
        {
            INFO("Destroying window \"%s\"...", window.title.c_str());
            SDL_DestroyWindow(window.handle);
        }

    }
}
