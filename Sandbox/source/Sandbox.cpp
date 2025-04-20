#include "Sandbox.h"
#include <Luma.h>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;

static SandboxState state;

void Sandbox_OnCreate()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f,  -0.5f, 0.0f, // right
        0.0f,  0.5f,  0.0f  // top
    };

    SetClearColor(0.12f, 0.12f, 0.12f);

    state.shader = LoadShader("assets/shaders/Default_vs.glsl", "assets/shaders/Default_fs.glsl");

    state.vertexArray = CreateVertexArray();
    state.vertexBuffer = CreateVertexBuffer();

    BindVertexArray(state.vertexArray);
    BindVertexBuffer(state.vertexBuffer);

    SetVertexBufferData(vertices, sizeof(vertices));
    SetVertexArrayAttribute(0, 3, 3 * sizeof(float), 0);

    UnbindVertexBuffer();
    UnbindVertexArray();
}

void Sandbox_OnUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE))
        QuitApplication();
}

void Sandbox_OnRender()
{
    BindShader(state.shader);

    BindVertexArray(state.vertexArray);
    RenderCommand::DrawArrays(3);
    UnbindVertexArray();

    UnbindShader();
}

void Sandbox_OnRenderUI()
{
}

void Sandbox_OnShutdown()
{
    UnloadShader(state.shader);

    DestroyVertexArray(state.vertexArray);
    DestroyVertexBuffer(state.vertexBuffer);
}
