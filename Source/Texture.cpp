#include "Texture.h"
#include "stb_image.h"

Minecraft::Texture::Texture(const std::string& path)
{
    LoadTextureFromFile(path.c_str(), &m_textureId, &m_textureWidth, &m_textureHeight);
}

bool Minecraft::Texture::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    stbi_set_flip_vertically_on_load(false);

    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

GLuint& Minecraft::Texture::GetTextureID()
{
    return m_textureId;
}

int& Minecraft::Texture::GetTextureWidth()
{
    return m_textureWidth;
}

int& Minecraft::Texture::GetTextureHeight()
{
    return m_textureHeight;
}
