#include "Sandbox.h"

#include <Luma.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;

static SandboxState state;
static u32 framebuffer = 0;
static u32 textureColorBuffer = 0;
static u32 textureDepthBuffer = 0;

void Sandbox_OnCreate()
{
    const ApplicationConfig& appInfo = GetApplicationInfo();

    SetClearColor(0.12f, 0.12f, 0.12f);

    state.defaultShader = &GetDefaultShader();
    state.uvShader = &GetUVShader();
    state.normalShader = &GetNormalShader();
    state.phongShader = &GetPhongShader();
    state.framebufferShader = &GetFramebufferShader();

    state.framebuffer = CreateFramebuffer(appInfo.windowWidth, appInfo.windowHeight);
    AddFramebufferAttachment(state.framebuffer, FB_ATTACHMENT_COLOR);
    AddFramebufferAttachment(state.framebuffer, FB_ATTACHMENT_DEPTH_STENCIL);
    ValidateFramebuffer(state.framebuffer);

    state.textures[0] = LoadTexture("assets/textures/texture0.png");
    state.textures[1] = LoadTexture("assets/textures/texture2.png");

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
    state.sun.color = glm::vec3(0.9f, 0.8f, 0.7f);

    state.framebufferMaterial = LoadMaterialDefault();
    state.framebufferMaterial.albedoTexture = &state.framebuffer.attachments[0];
    state.framebufferMaterial.shader = state.framebufferShader;

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
    BindFramebuffer(state.framebuffer);
    RendererClear(V3_OPEN(GetClearColor()));

    DrawLight(state.sun, *state.phongShader);

    DrawMesh(state.cubeMesh, glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.7f, 0.f)), state.materials[0]);
    DrawMesh(state.cubeMesh, glm::scale(glm::mat4(1.f), glm::vec3(7.f, 0.2f, 7.f)), state.materials[1]);
    UnbindFramebuffer();

    DrawMesh(state.quadMesh, glm::mat4(1.f), state.framebufferMaterial);
}

void Sandbox_OnRenderUI()
{
}

void Sandbox_OnShutdown()
{
    DestroyFramebuffer(state.framebuffer);
    DestroyMesh(state.quadMesh);
    DestroyMesh(state.cubeMesh);
}
