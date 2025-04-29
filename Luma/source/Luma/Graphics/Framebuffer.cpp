#include "Luma/Graphics/Framebuffer.h"
#include "Luma/Graphics/Texture.h"

#include "Luma/Core/Base.h"
#include "Luma/Core/Log.h"

#include <glad/glad.h>

namespace Luma
{
    namespace Graphics
    {
        u32 FramebufferTypeToGL(FramebufferType type);

        Framebuffer CreateFramebuffer(u32 width, u32 height)
        {
            Framebuffer framebuffer;
            framebuffer.width = width;
            framebuffer.height = height;

            glGenFramebuffers(1, &framebuffer.id);

            INFO("Framebuffer created successfully  with an ID of %d", framebuffer.id);
            return framebuffer;
        }

        void DestroyFramebuffer(Framebuffer& framebuffer)
        {
            if (framebuffer.id != 0)
            {
                INFO("Destroying framebuffer with an ID of %d...", framebuffer.id);
                glDeleteFramebuffers(1, &framebuffer.id);
                framebuffer.id = 0;
            }
        }

        void BindFramebuffer(const Framebuffer& framebuffer, FramebufferType type)
        {
            u32 glType = FramebufferTypeToGL(type);
            glBindFramebuffer(glType, framebuffer.id);
        }

        void UnbindFramebuffer(FramebufferType type)
        {
            u32 glType = FramebufferTypeToGL(type);
            glBindFramebuffer(glType, 0);
        }

        void AddFramebufferAttachment(Framebuffer& framebuffer, FramebufferAttachmentType type, bool isMultisampled)
        {
            TextureFormat textureFormat = TEXTURE_FORMAT_INVALID;
            u32 glType = 0;

            switch (type)
            {
                case FB_ATTACHMENT_COLOR:
                    glType = GL_COLOR_ATTACHMENT0 + framebuffer.numColorAttachments;
                    textureFormat = TEXTURE_FORMAT_RGB;
                    framebuffer.numColorAttachments++;
                    break;

                case FB_ATTACHMENT_DEPTH_STENCIL:
                    glType = GL_DEPTH_STENCIL_ATTACHMENT;
                    textureFormat = TEXTURE_FORMAT_DEPTH_STENCIL;
                    break;

                case FB_ATTACHMENT_DEPTH:
                    glType = GL_DEPTH_ATTACHMENT;
                    textureFormat = TEXTURE_FORMAT_DEPTH;
                    break;

                default:
                    break;
            }

            BindFramebuffer(framebuffer, FB_READ_WRITE);

            TextureSpecification textureSpec;
            textureSpec.width = framebuffer.width;
            textureSpec.height = framebuffer.height;
            textureSpec.format = textureFormat;

            if (isMultisampled)
            {
                Texture texture = LoadEmptyTextureMultisampled(textureSpec, 4);
                BindTexture(texture, 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, glType, GL_TEXTURE_2D_MULTISAMPLE, texture.id, 0);
                framebuffer.attachments.push_back(texture);
            }
            else
            {
                Texture texture = LoadEmptyTexture(textureSpec);
                BindTexture(texture, 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, glType, GL_TEXTURE_2D, texture.id, 0);
                framebuffer.attachments.push_back(texture);
            }

            UnbindFramebuffer(FB_READ_WRITE);
            UnbindTexture();
        }

        void ResizeFramebuffer(Framebuffer& framebuffer, u32 width, u32 height)
        {
            for (Texture& attachment : framebuffer.attachments)
            {
                u32 glSize = TextureFormatToGLSize(attachment.specification.format);
                u32 glFormat = TextureFormatToGL(attachment.specification.format);
                u32 internalFormat = TextureFormatToGLInternal(attachment.specification.format, false);
                u32 attachmentType = 0;

                switch (attachment.specification.format)
                {
                    case TEXTURE_FORMAT_RGB:
                        attachmentType = GL_COLOR_ATTACHMENT0 + framebuffer.numColorAttachments;
                        break;
                    case TEXTURE_FORMAT_DEPTH_STENCIL:
                        attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
                        break;

                    default:
                        break;
                }

                glBindTexture(GL_TEXTURE_2D, attachment.id);
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, glFormat, glSize, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, attachment.id, 0);
            }
        }

        void CopyFramebuffer(const Framebuffer& source, const Framebuffer& dest)
        {
            BindFramebuffer(source, FB_READ);
            BindFramebuffer(dest, FB_WRITE);
            glBlitFramebuffer(0, 0, source.width, source.height, 0, 0, dest.width, dest.height, GL_COLOR_BUFFER_BIT,
                              GL_NEAREST);
            UnbindFramebuffer(FB_READ);
            UnbindFramebuffer(FB_WRITE);
        }

        void ValidateFramebuffer(Framebuffer& framebuffer)
        {
            BindFramebuffer(framebuffer, FB_READ_WRITE);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                ERROR("Framebuffer with an ID of %d, or one of it's attachments were not created successfully",
                      framebuffer.id);

            UnbindFramebuffer(FB_READ_WRITE);
        }

        u32 FramebufferTypeToGL(FramebufferType type)
        {
            u32 glType = 0;

            switch (type)
            {
                case FB_READ:
                    glType = GL_READ_FRAMEBUFFER;
                    break;

                case FB_WRITE:
                    glType = GL_DRAW_FRAMEBUFFER;
                    break;

                case FB_READ_WRITE:
                    glType = GL_FRAMEBUFFER;
                    break;

                default:
                    break;
            }

            return glType;
        }
    }
}
