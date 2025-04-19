#include "Sandbox.h"
#include <Luma.h>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;

void Sandbox_OnCreate()
{
}

void Sandbox_OnUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE))
        QuitApplication();
}

void Sandbox_OnRender()
{
}

void Sandbox_OnRenderUI()
{
}

void Sandbox_OnShutdown()
{
}
