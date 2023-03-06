#ifndef FUNCTION_CPP
#define FUNCTION_CPP

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
//Include GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
//Include vector
#include <vector>

#include "TP1/variable.cpp"

//Viser le centre du plan avec la camera en prenant le vertices 0 + last vertice / 2 
void targetCameraPlan(std::vector<glm::vec3> &indexed_vertices )
{
    glm::vec3 centroid(0.0f);
    for (const auto& vertex : indexed_vertices) {
        centroid += vertex;
    }
    centroid /= static_cast<float>(indexed_vertices.size());

    camera_target = centroid - camera_position;
}


void initPlane(std::vector<unsigned short> &indices, std::vector<std::vector<unsigned short>> &triangles, std::vector<glm::vec3> &indexed_vertices,std::vector<glm::vec2> &uv, int resolution, int size, bool randomheight)
{
    indices.clear();
    triangles.clear();
    indexed_vertices.clear();
    uv.clear();

    int nbVertices = resolution * resolution; // Nombre de sommet au total
    float pas = size / (float)resolution;
    float x = 0, y = 0, z = 0;


    for (int i = 0; i < resolution + 1; i++)
    {
        for (int j = 0; j < resolution + 1; j++)
        {
            x = j* pas;
            y= randomheight? ( std::max((float) rand() / (RAND_MAX),0.f) ): 0;
            z = i * pas;
            indexed_vertices.push_back(glm::vec3(x - size/2.f,y, z-size/2.f));

        }
    }

    //Itère sur les carré de vertices pour en déduire les deux triangles qui les compose 
    for (int i = 0; i < resolution; i++) //hauteur
    {
        for (int j = 0; j < resolution; j++) //largeur
        {
            unsigned short bottomLeft = j + i * (resolution + 1);
            unsigned short bottomRight = bottomLeft + 1;
            unsigned short topLeft = bottomLeft + (resolution + 1);
            unsigned short topRight = topLeft + 1;

            triangles.push_back({bottomLeft, topLeft, bottomRight});
            triangles.push_back({topRight, topLeft, bottomRight});
        }
    }
    // Recupère les id des sommets des triangles from "triangles" in "indices"
    for (unsigned short i = 0; i < triangles.size(); i++) 
    {
        indices.push_back(triangles[i][0]);
        indices.push_back(triangles[i][1]);
        indices.push_back(triangles[i][2]);
    }

}

void computeUV(std::vector<glm::vec2> &uv, int resolution) 
{
    uv.clear();
    float uv_pas = 1 / (float)resolution;
    for (int i = 0; i < resolution + 1; i++)
    {
        for (int j = 0; j < resolution+ 1; j++)
        {
            float u = j * uv_pas;
            float v = i * uv_pas;
            uv.push_back(glm::vec2(1-u, v));
        }
    }

}
void computeNormals(const std::vector<glm::vec3>& vertices, const std::vector<unsigned short>& indices, std::vector<glm::vec3>& normals)
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