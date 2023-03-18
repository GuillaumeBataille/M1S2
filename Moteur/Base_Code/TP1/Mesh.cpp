#ifndef MESH_CPP
#define MESH_CPP
#include "../TP1/gameobject.cpp"

class GO_Mesh : public GameObject
{
public:
    // Constructeur de GO_Plane
    GO_Mesh(const std::string &filename)
    {
        this->type = 2;
        bool success = loadOFF(filename, vertices_, indices_, triangles_);
        if (!success)
        {
            std::cout << "Mesh load failed" << std::endl;
        }
        else
        {
            std::cout << "load mesh success" << std::endl;
        }
        this->GenerateBuffers();
    }
};
#endif