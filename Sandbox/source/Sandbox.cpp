#include "Sandbox.h"
#include <Luma.h>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;

static SandboxState state;

void Sandbox_OnCreate()
{
    Vertex vertices[] = {
        (Vertex){glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.f, 0.f)}, // bottom left
        (Vertex){glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.f, 0.f)},  // bottom right
        (Vertex){glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.f, 1.f)},   // top right
        (Vertex){glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.f, 1.f)}   // top left
    };

    u32 indices[] = {
        0, 1, 2, // first Triangle
        2, 3, 0  // second Triangle
    };

    SetClearColor(0.12f, 0.12f, 0.12f);

    state.texture = LoadTexture("assets/textures/texture2.png");
    state.shader = LoadShader("assets/shaders/Default_vs.glsl", "assets/shaders/Default_fs.glsl");

    state.vertexArray = CreateVertexArray();
    state.vertexBuffer = CreateBuffer();
    state.indexBuffer = CreateBuffer();

    BindVertexArray(state.vertexArray);
    BindVertexBuffer(state.vertexBuffer);
    BindIndexBuffer(state.indexBuffer);

    SetVertexBufferData(vertices, sizeof(vertices));
    SetIndexBufferData(indices, sizeof(indices));

    SetVertexArrayAttribute(0, 3, offsetof(Vertex, position));
    SetVertexArrayAttribute(1, 2, offsetof(Vertex, texCoord));

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
    BindTexture(state.texture, 0);

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
    DestroyBuffer(state.indexBuffer);
}
