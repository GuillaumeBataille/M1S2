#define STB_IMAGE_IMPLEMENTATION
#include "../common/stb_image.h"
#include <string>
#include <vector>
#include <GL/glew.h>

// Loader pour texture 2D - sampler2D
GLuint loadTexture(const char *path)
{
    printf("Reading image %s\n", path);
    //  Data read from the header of the image file
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

// Loader pour charger 6 faces de texture pour cube (aka Skybox)
GLuint loadTextureCube(std::vector<std::string> faces)
{
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // Load each face of the cube map
    int width, height, numChannels;
    for (GLuint i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &numChannels, 0);
        if (!data)
        {
            printf("%s could not be opened or is not a valid image file.\n", faces[i].c_str());
            return 0;
        }
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
            printf("%s has an unsupported number of channels.\n", faces[i].c_str());
            stbi_image_free(data);
            return 0;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        std::cout << "Reading cube map :" << i << " done." << std::endl;
    }

    // Set texture filtering options
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;
}

// Loader pour appliquer 6 fois la même texture sur un cube
GLuint loadTextureCubeMono(const std::string &facePath)
{
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // Load the texture for each face of the cube map
    int width, height, numChannels;
    unsigned char *data = stbi_load(facePath.c_str(), &width, &height, &numChannels, 0);
    if (!data)
    {
        printf("%s could not be opened or is not a valid image file.\n", facePath.c_str());
        return 0;
    }
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
        printf("%s has an unsupported number of channels.\n", facePath.c_str());
        stbi_image_free(data);
        return 0;
    }

    for (GLuint i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    stbi_image_free(data);

    // Set texture filtering options
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;
}

// Permet de return la partie Red d'une texture (used sur heightmap pour determiner les hauteurs coté CPU)
std::vector<unsigned char> loadHeightmap(const char *path, int &width, int &height, int &nrChannels)
{
    stbi_set_flip_vertically_on_load(true); // Inverse l'axe Y pour que l'origine soit en bas à gauche

    // Chargement de l'image
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb);
    if (!data)
    {
        std::cerr << "Impossible de charger l'image " << path << std::endl;
        exit(1);
    }
    // Récupération du canal rouge
    std::vector<unsigned char> result(width * height);
    for (int i = 0; i < width * height; i++)
    {
        result[i] = data[i * 3];
    }

    // Libération de la mémoire allouée par stb_image
    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);
    return result;
}

// Applique dans la composante Red des coordonnées uv pour return le niveau de gris associé (qu'on divise pas 255 pour une hauteur)
float getGrayLevel(const std::vector<unsigned char> &data, int width, int height, float u, float v)
{
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);
    // Clamp au cas ou on sort des limites du plan/image
    x = glm::clamp(x, 0, width - 1);
    y = glm::clamp(y, 0, height - 1);
    // On divise par 255 parce qu'on veut pas le niveau de gris mais la valeur identique a celle du GPU (entre 0 et 1)
    return static_cast<float>(data[y * width + x]) / 255.0f;
}
