#include "Luma/Graphics/Framebuffer.h"
#include "Luma/Graphics/Texture.h"

#include "Luma/Core/Base.h"
#include "Luma/Core/Log.h"

#include <glad/glad.h>

namespace Luma
{
    namespace Graphics
    {
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

        void BindFramebuffer(Framebuffer& framebuffer)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id);
        }

        void UnbindFramebuffer()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void AddFramebufferAttachment(Framebuffer& framebuffer, FramebufferAttachmentType type)
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

                default:
                    break;
            }

            BindFramebuffer(framebuffer);
            Texture texture = LoadEmptyTexture(textureFormat, framebuffer.width, framebuffer.height);
            glFramebufferTexture2D(GL_FRAMEBUFFER, glType, GL_TEXTURE_2D, texture.id, 0);
            UnbindFramebuffer();

            framebuffer.attachments.push_back(texture);
        }

        void ResizeFramebuffer(Framebuffer& framebuffer, u32 width, u32 height)
        {
            for (Texture& attachment : framebuffer.attachments)
            {
                u32 glSize = TextureFormatToGLSize(attachment.format);
                u32 glFormat = TextureFormatToGL(attachment.format);
                u32 internalFormat = TextureFormatToGLInternal(attachment.format, false);
                u32 attachmentType = 0;

                switch (attachment.format)
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

        void ValidateFramebuffer(Framebuffer& framebuffer)
        {
            BindFramebuffer(framebuffer);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                ERROR("Framebuffer with an ID of %d, or one of it's attachments were not created successfully",
                      framebuffer.id);

            UnbindFramebuffer();
        }
    }
}
