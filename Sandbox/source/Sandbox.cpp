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
void SetupCamera();
void SetupLights();
void SetupMaterials();

void RenderScene();
void RenderPassLight();

void Sandbox_OnCreate()
{
    const ApplicationConfig& appInfo = GetApplicationInfo();

    SetClearColor(0.12f, 0.12f, 0.12f);

    SetupShaders();
    SetupFramebuffers();
    SetupTextures();
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

    RenderPassLight();

    RenderCommand::WriteToDepth(false);

    /*
    glm::mat4 orthoProjection = glm::mat4(1.f);
    BindShader(*state.framebufferShader);
    SetShaderUniform(*state.framebufferShader, "projectionMatrix", &orthoProjection, SHADER_UNIFORM_MAT4);
    UnbindShader();

    DrawMesh(state.quadMesh, glm::mat4(1.f), state.framebufferMaterial);*/
}

void Sandbox_OnRenderUI()
{
    ImGui::DockSpaceOverViewport();

    ImGui::ShowDemoWindow();
    DisplaySceneViewport(state.framebufferIntermediate, *state.framebufferShader);
}

void Sandbox_OnShutdown()
{
    DestroyFramebuffer(state.framebuffer);
    DestroyFramebuffer(state.framebufferIntermediate);
}

void SetupShaders()
{
    state.defaultShader = &GetShaderDefault();
    state.uvShader = &GetShaderUV();
    state.normalShader = &GetShaderNormal();
    state.phongShader = &GetShaderPhong();
    state.framebufferShader = &GetShaderPostProcessing();
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
}

void SetupTextures()
{
    state.textures[0] = LoadTexture("assets/textures/texture0.png");
    state.textures[1] = LoadTexture("assets/textures/texture2.png");
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
    state.sun.intensity = 1.5f;
    state.sun.color = glm::vec3(0.9f, 0.8f, 0.7f);

    state.spotlight.position = glm::vec3(0.f, 5.f, 0.f);
    state.spotlight.target = glm::vec3(0.f);
    state.spotlight.color = glm::vec3(0.8f, 0.7f, 0.4f);
    state.spotlight.intensity = 1.2f;
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
    state.framebufferMaterial.albedoTexture = &state.framebufferIntermediate.attachments[0];
}

void RenderScene()
{
    UpdateLight(state.sun, *state.phongShader);
    UpdateLight(state.spotlight, *state.phongShader);

    Mesh& cubeMesh = GetMeshCube();
    RendererDrawMesh(cubeMesh, glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.7f, 0.f)), state.materials[0]);
    RendererDrawMesh(cubeMesh, glm::scale(glm::mat4(1.f), glm::vec3(20.f, 0.2f, 20.f)), state.materials[1]);

    RendererDrawCubeWires(glm::vec3(0.f, 0.7f, 0.f), glm::vec3(0.f), glm::vec3(1.f), Colors::Orange);
}

void RenderPassLight()
{
    const ApplicationConfig& appInfo = GetApplicationInfo();
    const ImVec2 aspectSize = GetViewportAspectSize();
    RenderCommand::SetViewport(aspectSize.x, aspectSize.y);

    BindFramebuffer(state.framebuffer, FB_READ_WRITE);
    RendererClear(V3_OPEN(GetClearColor()));

    for (u32 i = 0; i < LEN(state.materials); i++)
        state.materials[i].shader = state.phongShader;

    RenderScene();

    CopyFramebuffer(state.framebuffer, state.framebufferIntermediate);
    UnbindFramebuffer(FB_READ_WRITE);
}
