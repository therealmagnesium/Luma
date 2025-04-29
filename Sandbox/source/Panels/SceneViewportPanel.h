#pragma once
#include <Luma.h>
#include <imgui.h>

using namespace Luma::Core;
using namespace Luma::Graphics;
using namespace Luma::UI;

struct SceneViewportState
{
    Framebuffer* framebuffer = NULL;
    Shader* postProcessingShader = NULL;
    ImVec2 aspectSize;
};

ImVec2& GetViewportAspectSize();
void DisplaySceneViewport(Framebuffer& framebuffer, Shader& postProcessingShader);
