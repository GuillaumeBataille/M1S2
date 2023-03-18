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
// #include "common/my_texture.cpp"

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
        transform_global = glm::mat4();
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
        glBindVertexArray(vao_);

        glUniform1i(typeLoc, type);
        glUniformMatrix4fv(transformLoc, 1, false, &getFullTransform()[0][0]);
        glUniform4fv(colorLoc, 1, &color_[0]);

        /*GLuint id_v = glGetUniformLocation(shaderProgram_, "view_mat");
        GLuint id_p = glGetUniformLocation(shaderProgram_, "project_mat");
        mat_p = glm::perspective(glm::radians(45.0f), (float)4 / (float)3, 0.1f, 100.0f);
        mat_v = glm::lookAt(camera_position, camera_target, camera_up);
        glUniformMatrix4fv(id_v, 1, false, &mat_v[0][0]);
        glUniformMatrix4fv(id_p, 1, false, &mat_p[0][0]);*/

        glBindBuffer(GL_ARRAY_BUFFER, vboVertices_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(0);

        // Générer un VBO pour les coordonnées UV
        glBindBuffer(GL_ARRAY_BUFFER, vboUV);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(1);
        // Générer un VBO pour les indices
        // Draw les triangles et recurse le draw sur les enfants
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices_);
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0);

        for (const GameObject *child : children_)
        {
            child->draw();
        }
        glBindVertexArray(0);
        //  Desactiver les layouts après avoir dessiner
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

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
    }

    void activateTextures()
    {
        glActiveTexture(GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE1);
        glActiveTexture(GL_TEXTURE2);
        glActiveTexture(GL_TEXTURE3);
        glActiveTexture(GL_TEXTURE4);
        glActiveTexture(GL_TEXTURE5);
        glActiveTexture(GL_TEXTURE6);
    }

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
    // Buffers things
    GLuint vao_;
    GLuint vboVertices_;
    GLuint vboIndices_;
    GLuint vboUV;
    GLint transformLoc;
    GLint colorLoc;
    GLuint typeLoc;
    // GLuint shaderProgram_;
    int type = 0; // Type de l'objet
    // 0 = GameObject 1 = Plane 2 = Mesh
    // Hierachial structure - SceneGraph
    GameObject *parent_;
    std::vector<GameObject *> children_;
    // Data due to the current object loaded
    std::vector<glm::vec3> vertices_;
    std::vector<unsigned short> indices_;
    std::vector<glm::vec2> uvs_;
    std::vector<std::vector<unsigned short>> triangles_;
    // Added data by the user
    // GLuint texture_;
    glm::vec4 color_;
    // Transform local and global
    glm::mat4 transform_;
    glm::mat4 transform_global;
    float time;
};

#endif