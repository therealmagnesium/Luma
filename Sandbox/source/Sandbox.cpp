#include "Sandbox.h"
#include <Luma.h>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;

static SandboxState state;

void Sandbox_OnCreate()
{
    float vertices[] = {
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f  // top left
    };

    u32 indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    SetClearColor(0.12f, 0.12f, 0.12f);

    state.shader = LoadShader("assets/shaders/Default_vs.glsl", "assets/shaders/Default_fs.glsl");

    state.vertexArray = CreateVertexArray();
    state.vertexBuffer = CreateBuffer();
    state.indexBuffer = CreateBuffer();

    BindVertexArray(state.vertexArray);
    BindVertexBuffer(state.vertexBuffer);
    BindIndexBuffer(state.indexBuffer);

    SetVertexBufferData(vertices, sizeof(vertices));
    SetIndexBufferData(indices, sizeof(indices));
    SetVertexArrayAttribute(0, 3, 3 * sizeof(float), 0);

    UnbindVertexBuffer();
    UnbindIndexBuffer();
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
    BindIndexBuffer(state.indexBuffer);

    RenderCommand::DrawIndexed(6);

    UnbindIndexBuffer();
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
    DestroyBuffer(state.vertexBuffer);
}
