#pragma once
#include "Luma/Core/Application.h"
#include "Luma/Core/Log.h"

extern void LUMA_EntryPoint(Luma::Core::Application* app);

int main(int argc, char** argv)
{
    Luma::Core::Application app;
    LUMA_EntryPoint(&app);

    ASSERT(app.OnCreate != NULL && app.OnUpdate != NULL && app.OnRender != NULL && app.OnRenderUI != NULL &&
               app.OnShutdown != NULL,
           "The application could not find references to it's function pointers!");

    Luma::Core::SetupApplication(&app);
    Luma::Core::RunApplication();

    return 0;
}
