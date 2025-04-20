#include "Luma/Graphics/Texture.h"
#include "Luma/Core/Log.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Luma
{
    namespace Graphics
    {
        Texture LoadTexture(const char* path)
        {
            Texture texture;
            texture.path = path;

            glGenTextures(1, &texture.id);

            stbi_set_flip_vertically_on_load(true);
            u8* data = stbi_load(path, (s32*)&texture.width, (s32*)&texture.height, (s32*)&texture.numChannels, 0);

            if (data == NULL)
            {
                WARN("Failed to load texture \"%s\"", path);
                stbi_image_free(data);
                return texture;
            }

            switch (texture.numChannels)
            {
                case 1:
                    texture.format = TEXTURE_FORMAT_RED;
                    break;
                case 3:
                    texture.format = TEXTURE_FORMAT_RGB;
                    break;
                case 4:
                    texture.format = TEXTURE_FORMAT_RGBA;
                    break;
            }

            u32 glFormat = TextureFormatToGL(texture.format);

            glBindTexture(GL_TEXTURE_2D, texture.id);

            glTexImage2D(GL_TEXTURE_2D, 0, glFormat, texture.width, texture.height, 0, glFormat, GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(data);

            INFO("Texture \"%s\" loaded successfully with id %d", path, texture.id);
            return texture;
        }

        void BindTexture(Texture& texture, u8 slot)
        {
            glBindTexture(GL_TEXTURE_2D, texture.id);
            glActiveTexture(GL_TEXTURE0 + slot);
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
                default:
                    break;
            }

            return glFormat;
        }
    }
}
