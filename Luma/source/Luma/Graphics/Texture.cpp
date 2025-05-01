#include "Luma/Graphics/Texture.h"
#include "Luma/Core/Log.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Luma
{
    namespace Graphics
    {
        Texture LoadEmptyTexture(TextureSpecification& spec)
        {
            Texture texture;
            texture.specification.format = spec.format;
            texture.specification.width = spec.width;
            texture.specification.height = spec.height;
            texture.isMultisampled = false;

            u32 glFormat = TextureFormatToGL(texture.specification.format);
            u32 internalFormat = TextureFormatToGLInternal(texture.specification.format, false);
            u32 glSize = TextureFormatToGLSize(texture.specification.format);

            glGenTextures(1, &texture.id);
            glBindTexture(GL_TEXTURE_2D, texture.id);

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, spec.width, spec.height, 0, glFormat, glSize, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            /*
                    float borderColor[] = {1.f, 1.f, 1.f, 1.f};
                    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);*/

            glBindTexture(GL_TEXTURE_2D, 0);

            INFO("Empty texture loaded successfully with an ID of %d", texture.id);
            return texture;
        }

        Texture LoadEmptyTextureMultisampled(TextureSpecification& spec, u8 numSamples)
        {
            Texture texture;
            texture.specification.format = spec.format;
            texture.specification.width = spec.width;
            texture.specification.height = spec.height;
            texture.isMultisampled = true;

            glGenTextures(1, &texture.id);

            u32 glFormat = TextureFormatToGL(texture.specification.format);
            u32 internalFormat = TextureFormatToGLInternal(texture.specification.format, false);
            u32 glSize = TextureFormatToGLSize(texture.specification.format);

            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture.id);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, glFormat, spec.width, spec.height, true);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

            INFO("Empty multisampled texture loaded successfully with an ID of %d and %d samples", texture.id,
                 numSamples);
            return texture;
        }

        Texture LoadTexture(const char* path)
        {
            Texture texture;
            texture.path = path;

            glGenTextures(1, &texture.id);

            stbi_set_flip_vertically_on_load(true);
            u8* data = stbi_load(path, (s32*)&texture.specification.width, (s32*)&texture.specification.height,
                                 (s32*)&texture.numChannels, 0);

            if (data == NULL)
            {
                WARN("Failed to load texture \"%s\"", path);
                stbi_image_free(data);
                return texture;
            }

            switch (texture.numChannels)
            {
                case 1:
                    texture.specification.format = TEXTURE_FORMAT_RED;
                    break;
                case 3:
                    texture.specification.format = TEXTURE_FORMAT_RGB;
                    break;
                case 4:
                    texture.specification.format = TEXTURE_FORMAT_RGBA;
                    break;
            }

            u32 glFormat = TextureFormatToGL(texture.specification.format);
            u32 internalFormat = TextureFormatToGLInternal(texture.specification.format, true);

            glBindTexture(GL_TEXTURE_2D, texture.id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture.specification.width, texture.specification.height, 0,
                         glFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(data);

            INFO("Texture \"%s\" loaded successfully with an ID of %d", path, texture.id);
            return texture;
        }

        void BindTexture(const Texture& texture, u8 slot)
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, texture.id);
        }

        void UnbindTexture(u8 slot)
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        u32 TextureFormatToGL(TextureFormat format)
        {
            u32 glFormat = 0;

            switch (format)
            {
                case TEXTURE_FORMAT_RGB:
                    glFormat = GL_RGB;
                    break;

                case TEXTURE_FORMAT_RGBA:
                    glFormat = GL_RGBA;
                    break;

                case TEXTURE_FORMAT_RED:
                    glFormat = GL_RED;
                    break;

                case TEXTURE_FORMAT_DEPTH:
                    glFormat = GL_DEPTH_COMPONENT;
                    break;

                case TEXTURE_FORMAT_DEPTH_STENCIL:
                    glFormat = GL_DEPTH_STENCIL;
                    break;

                default:
                    break;
            }

            return glFormat;
        }

        u32 TextureFormatToGLInternal(TextureFormat format, bool applyGamma)
        {
            u32 glFormat = 0;

            switch (format)
            {
                case TEXTURE_FORMAT_RGB:
                    glFormat = (applyGamma) ? GL_SRGB : GL_RGB16F;
                    break;

                case TEXTURE_FORMAT_RGBA:
                    glFormat = (applyGamma) ? GL_SRGB_ALPHA : GL_RGBA16F;
                    break;

                case TEXTURE_FORMAT_RED:
                    glFormat = GL_RED;
                    break;

                case TEXTURE_FORMAT_DEPTH:
                    glFormat = GL_DEPTH_COMPONENT;
                    break;

                case TEXTURE_FORMAT_DEPTH_STENCIL:
                    glFormat = GL_DEPTH24_STENCIL8;
                    break;

                default:
                    break;
            }

            return glFormat;
        }

        u32 TextureFormatToGLSize(TextureFormat format)
        {
            u32 glSize = 0;
            switch (format)
            {
                case TEXTURE_FORMAT_RGB:
                    glSize = GL_FLOAT;
                    break;

                case TEXTURE_FORMAT_RGBA:
                    glSize = GL_FLOAT;
                    break;

                case TEXTURE_FORMAT_DEPTH:
                    glSize = GL_FLOAT;
                    break;

                case TEXTURE_FORMAT_DEPTH_STENCIL:
                    glSize = GL_UNSIGNED_INT_24_8;
                    break;

                default:
                    break;
            }

            return glSize;
        }
    }
}
