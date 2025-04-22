#pragma once
#include "Luma/Core/Base.h"
#include "Luma/Graphics/Texture.h"

#include <vector>

namespace Luma
{
    namespace Graphics
    {
        enum FramebufferAttachmentType : s32
        {
            FB_ATTACHMENT_INVALID = -1,
            FB_ATTACHMENT_COLOR,
            FB_ATTACHMENT_DEPTH,
            FB_ATTACHMENT_DEPTH_STENCIL,
        };

        struct Framebuffer
        {
            u32 id = 0;
            u32 width = 0;
            u32 height = 0;
            u32 numColorAttachments = 0;
            std::vector<Texture> attachments;
        };

        Framebuffer CreateFramebuffer(u32 width, u32 height);
        void DestroyFramebuffer(Framebuffer& framebuffer);
        void BindFramebuffer(Framebuffer& framebuffer);
        void UnbindFramebuffer();
        void AddFramebufferAttachment(Framebuffer& framebuffer, FramebufferAttachmentType type);
        void ValidateFramebuffer(Framebuffer& framebuffer);
    }
}
