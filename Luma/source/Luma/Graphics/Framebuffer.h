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

        enum FramebufferType : s32
        {
            FB_INVALID = -1,
            FB_READ,
            FB_WRITE,
            FB_READ_WRITE
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
        void BindFramebuffer(const Framebuffer& framebuffer, FramebufferType type);
        void UnbindFramebuffer(FramebufferType type);
        void AddFramebufferAttachment(Framebuffer& framebuffer, FramebufferAttachmentType type, bool isMultisampled);
        void ResizeFramebuffer(Framebuffer& framebuffer, u32 width, u32 height);
        void CopyFramebuffer(const Framebuffer& source, const Framebuffer& dest);
        void ValidateFramebuffer(Framebuffer& framebuffer);
    }
}
