#ifndef GAMEOBJECT_CPP
#define GAMEOBJECT_CPP

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <glm/glm.hpp>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include "../common/objloader.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <common/shader.hpp>
#include "../TP1/function.cpp"
#include "../common/my_texture.cpp"

class GameObject
{
public:
    // Generateur de buffers
    void GenerateBuffers()
    {
        // associer les shaders via un program pour pouvoir envoyer nos shaders
        glGenVertexArrays(1, &vao_);    // Le VAO qui englobe tout
        glGenBuffers(1, &vboVertices_); // VBO vertex
        glGenBuffers(1, &vboUV);        // VBO uv
        glGenBuffers(1, &vboIndices_);  // VBO d'élements indices pour draw triangles
        // Recupérer en local les location de transform_GO et isGO
        transformLoc = glGetUniformLocation(shaderProgram_, "transform_GO");
        colorLoc = glGetUniformLocation(shaderProgram_, "color_GO");
        typeLoc = glGetUniformLocation(shaderProgram_, "type");
        // Des Binds + Chargement des buffers avec les donnéees de l'objets courant
        // Vertices
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);
        // UV
        glBindBuffer(GL_ARRAY_BUFFER, vboUV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs_.size(), &uvs_[0], GL_STATIC_DRAW);
        // Indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices_.size(), &indices_[0], GL_STATIC_DRAW);
    }

    // Extermintateur de buffers
    void DeleteBuffers()
    {
        glDeleteBuffers(1, &vboVertices_);
        glDeleteBuffers(1, &vboIndices_);
        glDeleteBuffers(1, &vboUV);
        glDeleteVertexArrays(1, &vao_);
        glDeleteProgram(shaderProgram_);
        for (GameObject *child : children_)
        {
            child->DeleteBuffers();
        }
    };

    // Constructeur générique
    GameObject()
    {

        // Param d'un nouveau gameobject
        parent_ = nullptr;
        color_ = glm::vec4(0.3, 0.3, 0.5, 1);
        transform_ = glm::mat4();
        transform_global = glm::mat4(); // Uselesss pas encore implémenter
    }
    // Destructeur
    ~GameObject()
    {
        this->DeleteBuffers();
    }

    void setColor(const glm::vec4 &color) { color_ = color; }

    // Ajouter un child
    void addChild(GameObject *child)
    {
        child->parent_ = this;
        children_.push_back(child);
    }

    void translate(const glm::vec3 &translation)
    {
        transform_ = glm::translate(transform_, translation);
    }

    void scale(const glm::vec3 &scale)
    {
        transform_ = glm::scale(transform_, scale);
    }

    void rotate(float angle, const glm::vec3 &axis)
    {
        transform_ = glm::rotate(transform_, angle, axis);
    }

    virtual void draw() const
    {
        glBindVertexArray(vao_); // Bind le giga vecteur array

        // Envoi du type du GameObject
        glUniform1i(typeLoc, type);
        // Envoi de la matrice de transformation
        glUniformMatrix4fv(transformLoc, 1, false, &getFullTransform()[0][0]);
        // Envoi de la couleur du GameObject
        glUniform4fv(colorLoc, 1, &color_[0]);

        // Bind et Envoi de la liste pré chargée des vertices
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(0); // Dans le layout 0

        // Bind et Envoi de la liste pré chargée des UVs
        glBindBuffer(GL_ARRAY_BUFFER, vboUV);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(1); // Dans le layout 1

        // Bind et Draw les triangles et recurse le draw sur les enfants
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices_);
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0);
        for (const GameObject *child : children_)
        {
            child->draw();
        }
        glBindVertexArray(0); // Delink le VAO si plus tard on utilise plusieurs VAO (ça m'étonnerai ^^)
        //  Desactiver les layouts après avoir dessiner
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

    // Envoi de toute les textures, je l'appellerai que depuis le root une fois.
    // Attention - Changer de shaderProgram_ implique devoir recharger les textures
    virtual void GO_Texture()
    {
        {
            glActiveTexture(GL_TEXTURE0);
            loadTexture("../textures/HM2.bmp");
            glUniform1i(glGetUniformLocation(shaderProgram_, "texture0"), 0);

            glActiveTexture(GL_TEXTURE1);
            loadTexture("../textures/grass.bmp");
            glUniform1i(glGetUniformLocation(shaderProgram_, "texture1"), 1);

            glActiveTexture(GL_TEXTURE2);
            loadTexture("../textures/rock.bmp");
            glUniform1i(glGetUniformLocation(shaderProgram_, "texture2"), 2);

            glActiveTexture(GL_TEXTURE3);
            loadTexture("../textures/snowrock.bmp");
            glUniform1i(glGetUniformLocation(shaderProgram_, "texture3"), 3);

            glActiveTexture(GL_TEXTURE4);
            loadTexture("../textures/sun.jpg");
            glUniform1i(glGetUniformLocation(shaderProgram_, "texture4"), 4);

            glActiveTexture(GL_TEXTURE5);
            loadTexture("../textures/earth.jpg");
            glUniform1i(glGetUniformLocation(shaderProgram_, "texture5"), 5);

            glActiveTexture(GL_TEXTURE6);
            loadTexture("../textures/moon.jpg");
            glUniform1i(glGetUniformLocation(shaderProgram_, "texture6"), 6);
        }
        // -----Skybox-----//
        // On définit chacune des 6 faces
        std::vector<std::string> faces = {
            "../textures/skybox/right.jpg",
            "../textures/skybox/left.jpg",
            "../textures/skybox/top.jpg",
            "../textures/skybox/bottom.jpg",
            "../textures/skybox/front.jpg",
            "../textures/skybox/back.jpg"};
        glActiveTexture(GL_TEXTURE7);
        loadTextureCube(faces);
        glUniform1i(glGetUniformLocation(shaderProgram_, "texture7"), 7);
        //-------------//

        glActiveTexture(GL_TEXTURE8);
        loadTextureCubeMono("../textures/diamond.png"); // Mono va appliquer la texture sur toute les faces
        glUniform1i(glGetUniformLocation(shaderProgram_, "texture8"), 8);

        data = loadHeightmap("../textures/HM2.bmp", width, heights, nbrChannel);
    }
    virtual void rebuild(){}; // Reconstruire et recharger les buffers (notamment pour le +/- resolution dans input.cpp)

    virtual glm::vec3 getPos()
    {
        glm::vec4 position_homog = glm::vec4(vertices_[0], 1.0f); // Sussy de prendre le vertices[0] comme position de référence, centroid mieux mais flemme
        // Appliquez la transformation en multipliant la matrice de transformation et le vecteur de coordonnées homogènes
        glm::vec4 position_transformee_homog = getFullTransform() * position_homog;

        // Divisez les coordonnées x, y et z par w pour obtenir la position transformée
        glm::vec3 position_transformee = glm::vec3(position_transformee_homog) / position_transformee_homog.w;
        return position_transformee;
    }

    // Recupère les transformation des parents pour faire la matrice à send au shader
    glm::mat4 getFullTransform() const
    {
        if (parent_ == nullptr)
        {
            return transform_;
        }
        else
        {
            return parent_->getFullTransform() * transform_;
        }
    }
    // Test mais probablement useless
    void setMat_T(glm::vec3 T)
    {
        transform_ = glm::mat4();
        this->translate(T);
    }

    //------------- Buffers things
    GLuint vao_;
    GLuint vboVertices_;
    GLuint vboIndices_;
    GLuint vboUV;
    GLint transformLoc;
    GLint colorLoc;
    GLuint typeLoc;
    // GLuint shaderProgram_; // Now dans variable pour que toute le monde ai le même
    int type = 0; // Type de l'objet
    // 0 = GameObject 1 = Plane 2 = Mesh
    //--------- Hierachial structure - SceneGraph
    GameObject *parent_;
    std::vector<GameObject *> children_;
    //--------- Data due to the current object loaded
    std::vector<glm::vec3> vertices_;
    std::vector<unsigned short> indices_;
    std::vector<glm::vec2> uvs_;
    std::vector<std::vector<unsigned short>> triangles_;
    //----- Added data by the user
    glm::vec4 color_;
    //----- Transform local and global
    glm::mat4 transform_;
    glm::mat4 transform_global;
    // float time; Probalement pas une bonne idée de le stocker ici
};

#endif