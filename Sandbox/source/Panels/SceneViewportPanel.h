#pragma once
#include <Luma.h>

using namespace Luma::Core;
using namespace Luma::Graphics;
using namespace Luma::UI;

struct SceneViewportState
{
    Framebuffer* framebuffer = NULL;
    Shader* postProcessingShader = NULL;
};

void DisplaySceneViewport(Framebuffer& framebuffer, Shader& postProcessingShader);
