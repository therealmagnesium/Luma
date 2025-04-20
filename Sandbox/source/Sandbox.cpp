#include "Sandbox.h"

#include <Luma.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;

static SandboxState state;
static glm::mat4 transform = glm::mat4(1.f);

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

    state.camera.position = glm::vec3(0.f, 0.f, 3.f);
    state.camera.target = glm::vec3(0.f);
    state.camera.up = glm::vec3(0.f, 1.f, 0.f);
    state.camera.moveSpeed = 0.15f;
    state.camera.lookSensitivity = 3.f;
    SetPrimaryCamera(state.camera);

    state.mesh = CreateMesh(vertices, LEN(vertices), indices, LEN(indices));
}

void Sandbox_OnUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE))
        QuitApplication();

    UpdateCamera(state.camera, CAMERA_TYPE_ARCBALL);
}

void Sandbox_OnRender()
{
    DrawMesh(state.mesh, transform);
}

void Sandbox_OnRenderUI()
{
}

void Sandbox_OnShutdown()
{
}
