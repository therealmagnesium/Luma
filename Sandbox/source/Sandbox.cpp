#include "Sandbox.h"
#include "Panels/SceneViewportPanel.h"

#include <Luma.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include <imgui.h>

using namespace Luma;
using namespace Luma::Core;
using namespace Luma::Graphics;
using namespace Luma::UI;

static SandboxState state;

void SetupShaders();
void SetupFramebuffers();
void SetupTextures();
void SetupMeshes();
void SetupCamera();
void SetupLights();
void SetupMaterials();

void RenderScene();
void RenderPassColor();
void RenderPassShadow();

void Sandbox_OnCreate()
{
    const ApplicationConfig& appInfo = GetApplicationInfo();

    SetClearColor(0.12f, 0.12f, 0.12f);

    SetupShaders();
    SetupFramebuffers();
    SetupTextures();
    SetupMeshes();
    SetupCamera();
    SetupLights();
    SetupMaterials();
}

void Sandbox_OnUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE))
        QuitApplication();

    UpdateCamera(state.camera, CAMERA_TYPE_ARCBALL);
}

void Sandbox_OnRender()
{
    const ApplicationConfig& appInfo = GetApplicationInfo();

    RenderCommand::WriteToDepth(true);

    RenderPassShadow();
    RenderPassColor();

    RenderCommand::WriteToDepth(false);

    glm::mat4 orthoProjection = glm::mat4(1.f);
    BindShader(*state.framebufferShader);
    SetShaderUniform(*state.framebufferShader, "projectionMatrix", &orthoProjection, SHADER_UNIFORM_MAT4);
    UnbindShader();

    DrawMesh(state.quadMesh, glm::mat4(1.f), state.framebufferMaterial);
}

void Sandbox_OnRenderUI()
{
    /*
    ImGui::DockSpaceOverViewport();

    ImGui::ShowDemoWindow();
    DisplaySceneViewport(state.framebufferIntermediate, *state.framebufferShader);*/
}

void Sandbox_OnShutdown()
{
    DestroyFramebuffer(state.framebuffer);
    DestroyFramebuffer(state.framebufferIntermediate);
    DestroyFramebuffer(state.framebufferShadow);

    DestroyMesh(state.quadMesh);
    DestroyMesh(state.cubeMesh);
}

void SetupShaders()
{
    state.defaultShader = &GetDefaultShader();
    state.uvShader = &GetUVShader();
    state.normalShader = &GetNormalShader();
    state.phongShader = &GetPhongShader();
    state.shadowMapShader = &GetShadowMapShader();
    state.framebufferShader = &GetFramebufferShader();
}

void SetupFramebuffers()
{
    const ApplicationConfig& appInfo = GetApplicationInfo();

    state.framebuffer = CreateFramebuffer(appInfo.windowWidth, appInfo.windowHeight);
    AddFramebufferAttachment(state.framebuffer, FB_ATTACHMENT_COLOR, true);
    AddFramebufferAttachment(state.framebuffer, FB_ATTACHMENT_DEPTH_STENCIL, true);
    ValidateFramebuffer(state.framebuffer);

    state.framebufferIntermediate = CreateFramebuffer(appInfo.windowWidth, appInfo.windowHeight);
    AddFramebufferAttachment(state.framebufferIntermediate, FB_ATTACHMENT_COLOR, false);
    ValidateFramebuffer(state.framebufferIntermediate);

    state.framebufferShadow = CreateFramebuffer(2048, 2048);
    AddFramebufferAttachment(state.framebufferShadow, FB_ATTACHMENT_DEPTH, false);
    ValidateFramebuffer(state.framebufferShadow);
}

void SetupTextures()
{
    state.textures[0] = LoadTexture("assets/textures/texture0.png");
    state.textures[1] = LoadTexture("assets/textures/texture2.png");
}

void SetupMeshes()
{
    state.quadMesh = GenMeshQuad();
    state.cubeMesh = GenMeshCube();
}

void SetupCamera()
{
    state.camera.position = glm::vec3(0.f, 2.f, 5.f);
    state.camera.target = glm::vec3(0.f);
    state.camera.up = glm::vec3(0.f, 1.f, 0.f);
    state.camera.moveSpeed = 0.25f;
    state.camera.lookSensitivity = 7.f;
    SetPrimaryCamera(state.camera);
}

void SetupLights()
{
    state.sun.direction = glm::vec3(-0.2f, -1.f, -0.5f);
    state.sun.intensity = 2.f;
    state.sun.color = glm::vec3(0.9f, 0.8f, 0.7f);

    state.spotlight.position = glm::vec3(0.f, 5.f, 0.f);
    state.spotlight.target = glm::vec3(0.f);
    state.spotlight.color = glm::vec3(0.8f, 0.7f, 0.4f);
    state.spotlight.innerRadius = 15.f;
    state.spotlight.outerRadius = 20.f;
}

void SetupMaterials()
{
    state.defaultMaterial = LoadMaterialDefault();

    state.materials[0] = LoadMaterialDefault();
    // state.materials[0].albedo = glm::vec3(0.2f, 0.4f, 0.8f);
    state.materials[0].albedoTexture = &state.textures[0];
    state.materials[0].shader = state.phongShader;

    state.materials[1] = LoadMaterialDefault();
    // state.materials[1].albedo = glm::vec3(0.4f, 0.8f, 0.2f);
    state.materials[1].albedoTexture = &state.textures[1];
    state.materials[1].shader = state.phongShader;

    state.framebufferMaterial = LoadMaterialDefault();
    state.framebufferMaterial.shader = state.framebufferShader;
    state.framebufferMaterial.albedoTexture = &state.framebufferShadow.attachments[0];
}

void RenderScene()
{
    DrawLight(state.sun, *state.phongShader);
    DrawLight(state.spotlight, *state.phongShader);

    DrawMesh(state.cubeMesh, glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.7f, 0.f)), state.materials[0]);
    DrawMesh(state.cubeMesh, glm::scale(glm::mat4(1.f), glm::vec3(7.f, 0.2f, 7.f)), state.materials[1]);
}

void RenderPassColor()
{
    const ApplicationConfig& appInfo = GetApplicationInfo();
    RenderCommand::SetViewport(appInfo.windowWidth, appInfo.windowHeight);

    BindFramebuffer(state.framebuffer, FB_READ_WRITE);
    RendererClear(V3_OPEN(GetClearColor()));

    for (u32 i = 0; i < LEN(state.materials); i++)
        state.materials[i].shader = state.phongShader;

    RenderScene();

    CopyFramebuffer(state.framebuffer, state.framebufferIntermediate);
    UnbindFramebuffer(FB_READ_WRITE);
}

void RenderPassShadow()
{
    const ApplicationConfig& appInfo = GetApplicationInfo();
    const float projectionRange = 20.f;
    const float L = -projectionRange;
    const float R = projectionRange;
    const float B = -projectionRange;
    const float T = projectionRange;

    glm::mat4 lightProjection = glm::ortho(L, R, B, T, 0.1f, 10.f);
    glm::mat4 lightView = glm::lookAt(5.f * glm::vec3(0.5f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    RenderCommand::SetViewport(state.framebufferShadow.width, state.framebufferShadow.height);

    BindFramebuffer(state.framebufferShadow, FB_READ_WRITE);
    RendererClear(V3_OPEN(GetClearColor()));

    BindShader(*state.shadowMapShader);
    SetShaderUniform(*state.shadowMapShader, "lightSpaceMatrix", &lightSpaceMatrix, SHADER_UNIFORM_MAT4);
    UnbindShader();

    for (u32 i = 0; i < LEN(state.materials); i++)
        state.materials[i].shader = state.shadowMapShader;

    RenderScene();

    UnbindFramebuffer(FB_READ_WRITE);
}
