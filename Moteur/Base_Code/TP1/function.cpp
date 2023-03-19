#ifndef FUNCTION_CPP
#define FUNCTION_CPP

#include "../TP1/variable.cpp"
#include <glm/glm.hpp>

// Initie plan en fillant indices, vertices, triangles, uvs
// taille et res en param
void initPlane(std::vector<unsigned short> &indices, std::vector<std::vector<unsigned short>> &triangles, std::vector<glm::vec3> &indexed_vertices, std::vector<glm::vec2> &indexed_uvs, int resolution, int size, bool randomheight)
{
    indices.clear();
    triangles.clear();
    indexed_vertices.clear();
    indexed_uvs.clear();

    int nbVertices = resolution * resolution;
    float step = size / (float)resolution;
    float x, y, z, u, v;

    for (int i = 0; i <= resolution; i++)
    {
        for (int j = 0; j <= resolution; j++)
        {
            x = j * step;
            if (randomheight)
            {
                y = std::max((float)rand() / (RAND_MAX), 0.f);
            }
            else
            {
                y = 0;
            }
            z = i * step;
            u = x / size;
            v = z / size;
            indexed_vertices.push_back(glm::vec3(x - size / 2.f, y, z - size / 2.f));
            indexed_uvs.push_back(glm::vec2(u, v));
        }
    }

    for (int i = 0; i < resolution; i++)
    {
        for (int j = 0; j < resolution; j++)
        {
            unsigned short bottomLeft = j + i * (resolution + 1);
            unsigned short bottomRight = bottomLeft + 1;
            unsigned short topLeft = bottomLeft + (resolution + 1);
            unsigned short topRight = topLeft + 1;

            triangles.push_back({bottomLeft, bottomRight, topLeft});
            triangles.push_back({bottomRight, topRight, topLeft});

            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
            indices.push_back(topLeft);
            indices.push_back(bottomRight);
            indices.push_back(topRight);
            indices.push_back(topLeft);
        }
    }
}

void computeUV(std::vector<glm::vec2> &uv, int resolution)
{
    uv.clear();
    float uv_pas = 1 / (float)resolution;
    for (int i = 0; i < resolution + 1; i++)
    {
        for (int j = 0; j < resolution + 1; j++)
        {
            float u = j * uv_pas;
            float v = i * uv_pas;
            uv.push_back(glm::vec2(u, v));
        }
    }
}

void create_sphere(std::vector<unsigned short> &indices, std::vector<glm::vec3> &indexed_vertices, float radius, int resolution)
{
    indices.clear();
    indexed_vertices.clear();

    for (int i = 0; i <= resolution; ++i)
    {
        float theta1 = M_PI * (i / (float)resolution);
        float sin_theta1 = sin(theta1);
        float cos_theta1 = cos(theta1);

        for (int j = 0; j <= resolution; ++j)
        {
            float phi = 2 * M_PI * (j / (float)resolution);
            float sin_phi = sin(phi);
            float cos_phi = cos(phi);

            glm::vec3 vertex = glm::vec3(cos_phi * sin_theta1, cos_theta1, sin_phi * sin_theta1);
            indexed_vertices.push_back(vertex * radius);
        }
    }

    int num_vertices = indexed_vertices.size();
    for (int i = 0; i < resolution; ++i)
    {
        for (int j = 0; j < resolution; ++j)
        {
            int i1 = i * (resolution + 1) + j;
            int i2 = i1 + 1;
            int i3 = (i + 1) * (resolution + 1) + j;
            int i4 = i3 + 1;

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);

            indices.push_back(i2);
            indices.push_back(i3);
            indices.push_back(i4);
        }
    }
};

void create_cube(std::vector<unsigned short int> &indices, std::vector<glm::vec3> &vertices, float size)
{
    float half_size = size / 2.0f;

    // Vertices
    glm::vec3 v1(-half_size, -half_size, -half_size);
    glm::vec3 v2(half_size, -half_size, -half_size);
    glm::vec3 v3(half_size, half_size, -half_size);
    glm::vec3 v4(-half_size, half_size, -half_size);
    glm::vec3 v5(-half_size, -half_size, half_size);
    glm::vec3 v6(half_size, -half_size, half_size);
    glm::vec3 v7(half_size, half_size, half_size);
    glm::vec3 v8(-half_size, half_size, half_size);

    // Ajouter les vertices à la liste
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);
    vertices.push_back(v8);

    // Indices pour les faces
    indices = {
        0, 1, 2, 0, 2, 3, // face avant
        1, 5, 6, 1, 6, 2, // face droite
        4, 0, 3, 4, 3, 7, // face gauche
        5, 4, 7, 5, 7, 6, // face arriere
        3, 2, 6, 3, 6, 7, // face haut
        4, 5, 1, 4, 1, 0  // face bas
    };
}

std::vector<glm::vec3> computeCubeUVs()
{
    std::vector<glm::vec3> uvs(24);

    // Define the directions of the cube
    glm::vec3 directions[] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)};

    // Compute the UVs for each direction
    for (int i = 0; i < 6; i++)
    {
        uvs[i * 4 + 0] = glm::vec3(directions[i].x, directions[i].y, directions[i].z);
        uvs[i * 4 + 1] = glm::vec3(directions[i].x, directions[i].y, directions[i].z);
        uvs[i * 4 + 2] = glm::vec3(directions[i].x, directions[i].y, directions[i].z);
        uvs[i * 4 + 3] = glm::vec3(directions[i].x, directions[i].y, directions[i].z);
    }

    return uvs;
}

// Pas use pour l'instant mais servira
void computeNormals(const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices, std::vector<glm::vec3> &normals)
{
    normals.clear();
    normals.resize(vertices.size(), glm::vec3(0, 0, 0));
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        glm::vec3 v1 = vertices[indices[i]];
        glm::vec3 v2 = vertices[indices[i + 1]];
        glm::vec3 v3 = vertices[indices[i + 2]];
        glm::vec3 normal = glm::cross(v2 - v1, v3 - v1);
        normals[indices[i]] += normal;
        normals[indices[i + 1]] += normal;
        normals[indices[i + 2]] += normal;
    }
    for (size_t i = 0; i < normals.size(); i++)
    {
        normals[i] = glm::normalize(normals[i]);
    }
}
// Prends une position et le coin inf et coin sup d'un plan pour determiner les uv par projection sur le plan.
glm::vec2 calculateUV(const glm::vec3 &position, const glm::vec3 &bottomLeft, const glm::vec3 &topRight)
{
    glm::vec2 uv;
    float u = (position.x - bottomLeft.x) / (topRight.x - bottomLeft.x);
    float v = (position.z - bottomLeft.z) / (topRight.z - bottomLeft.z);
    uv.x = u;
    uv.y = 1 - v;
    return uv;
}
#endif