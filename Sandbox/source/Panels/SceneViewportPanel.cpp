#include "SceneViewportPanel.h"

#include <Luma.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace Luma::Core;
using namespace Luma::Graphics;
using namespace Luma::UI;

static SceneViewportState state;

ImVec2 GetLargestViewportSize();
ImVec2 GetCenteredViewportPosition(ImVec2 aspectSize);
void DrawCallback(const ImDrawList*, const ImDrawCmd*);

void DisplaySceneViewport(Framebuffer& framebuffer, Shader& postProcessingShader)
{
    state.framebuffer = &framebuffer;
    state.postProcessingShader = &postProcessingShader;

    ImGui::Begin("Scene Viewport");

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddCallback(DrawCallback, NULL);

    ImVec2 aspectSize = GetLargestViewportSize();
    ImVec2 windowPosition = GetCenteredViewportPosition(aspectSize);

    RenderCommand::SetViewport(aspectSize.x, aspectSize.y);
    ResizeFramebuffer(framebuffer, aspectSize.x, aspectSize.y);

    ImGui::SetCursorPos(windowPosition);
    ImGui::Image((u64)framebuffer.attachments[0].id, aspectSize, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));

    drawList->AddCallback(ImDrawCallback_ResetRenderState, NULL);
    ImGui::End();
}

ImVec2 GetLargestViewportSize()
{
    const ApplicationConfig& appInfo = GetApplicationInfo();
    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    float aspectRatio = appInfo.windowWidth / (float)appInfo.windowHeight;
    ImVec2 aspect = ImVec2(windowSize.x, windowSize.x / aspectRatio);

    if (aspect.y > windowSize.y)
    {
        aspect.y = windowSize.y;
        aspect.x = aspect.y * aspectRatio;
    }

    return aspect;
}

ImVec2 GetCenteredViewportPosition(ImVec2 aspectSize)
{
    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    ImVec2 viewportPosition;
    viewportPosition.x = (windowSize.x / 2.f) - (aspectSize.x / 2.f);
    viewportPosition.y = (windowSize.y / 2.f) - (aspectSize.y / 2.f);

    return viewportPosition;
}

void DrawCallback(const ImDrawList*, const ImDrawCmd*)
{
    const u32 albedoSlot = 0;
    const ImDrawData* draw_data = ImGui::GetDrawData();
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    const glm::mat4 projection = glm::ortho(L, R, B, T);

    BindShader(*state.postProcessingShader);
    BindTexture(state.framebuffer->attachments[0]);
    SetShaderUniform(*state.postProcessingShader, "albedoTexture", (void*)&albedoSlot, SHADER_UNIFORM_INT);
    SetShaderUniform(*state.postProcessingShader, "projectionMatrix", (void*)&projection, SHADER_UNIFORM_MAT4);
}
