#pragma once
#include <Luma.h>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;

struct SandboxState
{
    Shader* defaultShader = NULL;
    Shader* uvShader = NULL;
    Shader* normalShader = NULL;
    Camera camera;
    Mesh quadMesh;
    Mesh cubeMesh;
    Material materials[2];
    Texture textures[2];
};

void Sandbox_OnCreate();
void Sandbox_OnUpdate();
void Sandbox_OnRender();
void Sandbox_OnRenderUI();
void Sandbox_OnShutdown();
