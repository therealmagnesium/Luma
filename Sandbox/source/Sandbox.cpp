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

    state.defaultShader = &GetDefaultShader();
    state.uvShader = &GetUVShader();
    state.normalShader = &GetNormalShader();
    state.phongShader = &GetPhongShader();

    state.textures[0] = LoadTexture("assets/textures/texture0.png");
    state.textures[1] = LoadTexture("assets/textures/color_grid.png");

    state.quadMesh = GenMeshQuad();
    state.cubeMesh = GenMeshCube();

    state.camera.position = glm::vec3(0.f, 2.f, 5.f);
    state.camera.target = glm::vec3(0.f);
    state.camera.up = glm::vec3(0.f, 1.f, 0.f);
    state.camera.moveSpeed = 0.15f;
    state.camera.lookSensitivity = 3.f;
    SetPrimaryCamera(state.camera);

    state.sun.direction = glm::vec3(-0.2f, -1.f, -0.3f);
    state.sun.intensity = 2.f;

    state.materials[0] = LoadMaterialDefault();
    state.materials[0].albedoTexture = &state.textures[0];
    state.materials[0].shader = state.phongShader;

    state.materials[1] = LoadMaterialDefault();
    state.materials[1].albedoTexture = &state.textures[1];
    state.materials[1].shader = state.phongShader;
}

void Sandbox_OnUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE))
        QuitApplication();

    if (IsKeyPressed(KEY_1))
        for (u32 i = 0; i < LEN(state.materials); i++)
            state.materials[i].shader = state.phongShader;

    if (IsKeyPressed(KEY_2))
        for (u32 i = 0; i < LEN(state.materials); i++)
            state.materials[i].shader = state.defaultShader;

    if (IsKeyPressed(KEY_3))
        for (u32 i = 0; i < LEN(state.materials); i++)
            state.materials[i].shader = state.uvShader;

    if (IsKeyPressed(KEY_4))
        for (u32 i = 0; i < LEN(state.materials); i++)
            state.materials[i].shader = state.normalShader;

    UpdateCamera(state.camera, CAMERA_TYPE_ARCBALL);
}

void Sandbox_OnRender()
{
    DrawLight(state.sun, *state.phongShader);

    DrawMesh(state.cubeMesh, glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.7f, 0.f)), state.materials[0]);
    DrawMesh(state.cubeMesh, glm::scale(glm::mat4(1.f), glm::vec3(7.f, 0.2f, 7.f)), state.materials[1]);
}

void Sandbox_OnRenderUI()
{
}

void Sandbox_OnShutdown()
{
    DestroyMesh(state.quadMesh);
    DestroyMesh(state.cubeMesh);
}
