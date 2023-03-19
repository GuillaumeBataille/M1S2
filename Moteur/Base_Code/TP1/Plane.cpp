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
        // computeUV(uvs_, res);
        // Generer et charger les buffers
        this->GenerateBuffers();
        // Charger et envoyer les textures du plan
        // this->activateTextures();
    }
    // Rebuild le plane (quand on + ou - pour augmenter / baisser la resolution)
    void rebuild(int res)
    {
        this->resolution = res;
        initPlane(indices_, triangles_, vertices_, uvs_, this->resolution, size, false);
        // computeUV(uvs_, this->resolution);
        this->GenerateBuffers();
    }
    virtual void draw() const
    {
        // Old, mais il faudrait que j'essaye de le fix.
        // Permet qu'un plan très grand conserve des hauteur grande
        GLuint HeightLoc = glGetUniformLocation(shaderProgram_, "heightcoeff");
        glUniform1f(HeightLoc, sqrt(size));
        GameObject::draw(); // Appel a super draw
    };
};
#endif