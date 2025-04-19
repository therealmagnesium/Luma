#include "Sandbox.h"
#include <Luma/EntryPoint.h>

using namespace Luma;
using namespace Luma::Core;

void LUMA_EntryPoint(Application* app)
{
    ApplicationConfig config;
    config.name = "Sandbox";
    config.author = "Magnus Ahlstromer V";
    config.windowWidth = 1600;
    config.windowHeight = 900;

    app->config = config;
    app->OnCreate = Sandbox_OnCreate;
    app->OnUpdate = Sandbox_OnUpdate;
    app->OnRender = Sandbox_OnRender;
    app->OnRenderUI = Sandbox_OnRenderUI;
    app->OnShutdown = Sandbox_OnShutdown;
}
