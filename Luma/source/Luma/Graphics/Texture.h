#pragma once
#include "Luma/Core/Base.h"
#include <string>

namespace Luma
{
    namespace Graphics
    {
        enum TextureFormat : s32
        {
            TEXTURE_FORMAT_INVALID = -1,
            TEXTURE_FORMAT_RGB,
            TEXTURE_FORMAT_RGBA,
            TEXTURE_FORMAT_RED,
            TEXTURE_FORMAT_DEPTH,
            TEXTURE_FORMAT_DEPTH_STENCIL,
        };

        struct Texture
        {
            u32 id = 0;
            u32 width = 0;
            u32 height = 0;
            u32 numChannels = 0;
            std::string path = "";
            TextureFormat format = TEXTURE_FORMAT_INVALID;
        };

        Texture LoadEmptyTexture(TextureFormat format, u32 width, u32 height);
        Texture LoadTexture(const char* path);
        void BindTexture(Texture& texture);
        void UnbindTexture();

        u32 TextureFormatToGL(TextureFormat format);
        u32 TextureFormatToGLInternal(TextureFormat format, bool applyGamma);
        u32 TextureFormatToGLSize(TextureFormat format);
    }
}
