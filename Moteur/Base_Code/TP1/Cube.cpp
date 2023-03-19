#ifndef CUBE_CPP
#define CUBE_CPP
#include "../TP1/gameobject.cpp"

class GO_Cube : public GameObject
{
public:
    float size;
    int textureCubeid;
    std::vector<glm::vec3> UV_cube;
    GLuint vboUV_cube;
    // Constructeur de GO_Plane
    GO_Cube(float size, int Texid = 0)
    {
        this->size = size;
        this->textureCubeid = Texid;
        this->type = 4;
        // Init le plan + uvs inclus
        create_cube(indices_, vertices_, size);
        this->UV_cube = computeCubeUVs();
        // Generer et charger les buffers
        this->GenerateBuffers();
        glGenBuffers(1, &vboUV_cube);
        glBindBuffer(GL_ARRAY_BUFFER, vboUV_cube);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * UV_cube.size(), &UV_cube[0], GL_STATIC_DRAW);
    }

    virtual void draw() const
    {
        // Envoi de texid pour savoir quelle trexture appliquer sur le cube courant
        glBindVertexArray(vao_);
        GLuint TexidLoc = glGetUniformLocation(shaderProgram_, "Texid");
        glUniform1i(TexidLoc, textureCubeid);

        // Envoi les textures des cubes spécifique car uv = vec3 et non vec2
        glBindBuffer(GL_ARRAY_BUFFER, vboUV_cube);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(2);
        GameObject::draw(); // appel a super draw
    };
};
#endif