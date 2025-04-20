#pragma once
#include <Luma.h>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;

struct SandboxState
{
    u32 vertexArray;
    u32 vertexBuffer;
    Shader shader;
};

void Sandbox_OnCreate();
void Sandbox_OnUpdate();
void Sandbox_OnRender();
void Sandbox_OnRenderUI();
void Sandbox_OnShutdown();
