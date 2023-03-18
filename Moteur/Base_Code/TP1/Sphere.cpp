#ifndef SPHERE_CPP
#define SPHERE_CPP
#include "../TP1/gameobject.cpp"

class GO_Sphere : public GameObject
{
public:
    int resolution;
    int radius;
    int textureSphereid;
    // Constructeur de GO_Plane
    GO_Sphere(unsigned int res, unsigned int size, int Texid = 0)
    {
        this->resolution = res;
        this->radius = size;
        this->type = 3;
        textureSphereid = Texid;
        // sun = 0 earth = 1, moon = 2
        // Init le plan
        create_sphere(indices_, vertices_, size, resolution);
        // Compute les UV du plan
        computeUV(uvs_, res);
        // Generer et charger les buffers
        this->GenerateBuffers();
    }

    virtual void draw() const
    {
        GLuint TexidLoc = glGetUniformLocation(shaderProgram_, "Texid");
        glUniform1i(TexidLoc, textureSphereid);
        GameObject::draw();
    };
};
#endif