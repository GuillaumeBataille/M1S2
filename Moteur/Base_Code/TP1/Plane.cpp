#ifndef PLANE_CPP
#define PLANE_CPP
#include "../TP1/gameobject.cpp"

class GO_Plane : public GameObject
{
public:
    int resolution;
    int size;

    // Constructeur de GO_Plane
    GO_Plane(unsigned int res, unsigned int size)
    {
        this->resolution = res;
        this->size = size;
        this->type = 1;
        // Init le plan
        initPlane(indices_, triangles_, vertices_, uvs_, res, size, false);
        // Compute les UV du plan
        computeUV(uvs_, res);
        // Generer et charger les buffers
        this->GenerateBuffers();
        // Charger et envoyer les textures du plan
        this->activateTextures();
    }

    virtual void draw() const
    {
        GLuint HeightLoc = glGetUniformLocation(shaderProgram_, "heightcoeff");
        glUniform1f(HeightLoc, sqrt(size));
        GameObject::draw();
    };
};
#endif