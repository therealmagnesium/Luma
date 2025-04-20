#include "Sandbox.h"

#include <Luma.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;

static SandboxState state;

void Sandbox_OnCreate()
{
    SetClearColor(0.12f, 0.12f, 0.12f);

    state.camera.position = glm::vec3(0.f, 2.f, 5.f);
    state.camera.target = glm::vec3(0.f);
    state.camera.up = glm::vec3(0.f, 1.f, 0.f);
    state.camera.moveSpeed = 0.15f;
    state.camera.lookSensitivity = 3.f;
    SetPrimaryCamera(state.camera);

    state.textures[0] = LoadTexture("assets/textures/texel_checker.png");
    state.textures[1] = LoadTexture("assets/textures/small_checker.png");
    state.quadMesh = GenMeshQuad();
    state.cubeMesh = GenMeshCube();

    state.materials[0] = LoadMaterialDefault();
    state.materials[0].albedoTexture = &state.textures[0];

    state.materials[1] = LoadMaterialDefault();
    state.materials[1].albedoTexture = &state.textures[1];
}

void Sandbox_OnUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE))
        QuitApplication();

    UpdateCamera(state.camera, CAMERA_TYPE_ARCBALL);
}

void Sandbox_OnRender()
{
    DrawMesh(state.cubeMesh, glm::scale(glm::mat4(1.f), glm::vec3(5.f, 0.2f, 5.f)), state.materials[0]);
    DrawMesh(state.quadMesh, glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, 0.f)), state.materials[1]);
}

void Sandbox_OnRenderUI()
{
}

void Sandbox_OnShutdown()
{
    DestroyMesh(state.quadMesh);
    DestroyMesh(state.cubeMesh);
}
