#ifndef FUNCTION_CPP
#define FUNCTION_CPP

#include "TP1/variable.cpp"

// Viser le centre du plan avec la camera en prenant le vertices 0 + last vertice / 2
void targetCameraPlan(std::vector<glm::vec3> &indexed_vertices)
{
    glm::vec3 centroid(0.0f);
    for (const auto &vertex : indexed_vertices)
    {
        centroid += vertex;
    }
    centroid /= static_cast<float>(indexed_vertices.size());

    camera_target = centroid - camera_position;
}

void initPlane(std::vector<unsigned short> &indices, std::vector<std::vector<unsigned short>> &triangles, std::vector<glm::vec3> &indexed_vertices, std::vector<glm::vec2> &uv, int resolution, int size, bool randomheight)
{
    indices.clear();
    triangles.clear();
    indexed_vertices.clear();
    uv.clear();

    int nbVertices = resolution * resolution;
    float step = size / (float)resolution;
    float x, y, z;

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
            indexed_vertices.push_back(glm::vec3(x - size / 2.f, y, z - size / 2.f));
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

void sphericalMapping(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs)
{
    for (const auto &v : vertices)
    {
        // Formule internet pour mapper une sphère
        float theta = std::atan2(v.z, v.x);
        float phi = std::acos(v.y / glm::length(v));
        uvs.emplace_back(theta / (2 * M_PI) + 0.5, phi / M_PI);
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
#endif