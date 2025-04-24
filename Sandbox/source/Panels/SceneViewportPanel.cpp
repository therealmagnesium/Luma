#include "SceneViewportPanel.h"
#include <Luma.h>

#include <imgui.h>

using namespace Luma::Core;
using namespace Luma::Graphics;
using namespace Luma::UI;

ImVec2 GetLargestViewportSize();
ImVec2 GetCenteredViewportPosition(ImVec2 aspectSize);
void DrawCallback(const ImDrawList*, const ImDrawCmd*);

void DisplaySceneViewport(Framebuffer& framebuffer, Shader& postProcessingShader)
{
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
}
