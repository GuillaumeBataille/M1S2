#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"
#include <GL/glew.h>

GLuint loadTexture(const char *path)
{
    printf("Reading image %s\n", path);
    // Data read from the header of the image file
    int width, height, numChannels;
    unsigned char *data = stbi_load(path, &width, &height, &numChannels, 0);

    if (!data)
    {
        printf("%s could not be opened or is not a valid image file.\n", path);
        return 0;
    }

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    GLenum format;
    if (numChannels == 1)
    {
        format = GL_RED;
    }
    else if (numChannels == 3)
    {
        format = GL_RGB;
    }
    else if (numChannels == 4)
    {
        format = GL_RGBA;
    }
    else
    {
        printf("%s has an unsupported number of channels.\n", path);
        stbi_image_free(data);
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Set texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}