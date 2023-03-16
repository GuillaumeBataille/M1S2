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
#include <common/objloader.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.hpp>
#include "common/my_texture.cpp"

class Mesh
{
public:
    // Constructeur
    Mesh(const std::string &filename)
    {

        if (filename != "")
        {

            bool success = loadOFF(filename, vertices_, indices_, triangles_);
            if (!success)
            {
                std::cout << "Mesh load failed" << std::endl;
            }
            parent_ = nullptr;
            // Generate and bind VAO
            glGenVertexArrays(1, &vao_);
            glBindVertexArray(vao_);

            // Generate and bind VBO for vertices
            glGenBuffers(1, &vboVertices_);
            glBindBuffer(GL_ARRAY_BUFFER, vboVertices_);
            glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), vertices_.data(), GL_STATIC_DRAW);

            // Set vertex attribute pointers
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);

            // Generate and bind VBO for indices
            glGenBuffers(1, &vboIndices_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned short), indices_.data(), GL_STATIC_DRAW);
        }

        // Initialize transformation matrix to identity
        transform_ = glm::mat4(1.0f);
        // Initialize texture and color
        // texture_ = 0;
        // color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        // Load shaders
        shaderProgram_ = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");
    }
    // Destructeur
    ~Mesh()
    {
        glDeleteBuffers(1, &vboVertices_);
        glDeleteBuffers(1, &vboIndices_);
        glDeleteVertexArrays(1, &vao_);
        glDeleteProgram(shaderProgram_);
        for (Mesh *child : children_)
        {
            delete child;
        }
    }

    // void setColor(const glm::vec4 &color) { color_ = color; }

    // Ajouter un child
    void addChild(Mesh *child)
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

    void draw() const
    {
        glBindVertexArray(vao_);

        // Apply transformation to vertices
        GLint transformLoc = glGetUniformLocation(shaderProgram_, "transform_");
        GLint isMeshLoc = glGetUniformLocation(shaderProgram_, "isMesh");
        // GLint colorLoc = glGetUniformLocation(shaderProgram_, "color_M");
        // glUniform4fv(colorLoc, 1, &color_[0]);
        glUniformMatrix4fv(transformLoc, 1, false, &getFullTransform()[0][0]);
        glUniform1i(isMeshLoc, true);
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0);
        for (const Mesh *child : children_)
        {
            child->draw();
        }
    }

    // Setters and getters for transformation matrix
    void setTransform(const glm::mat4 &transform) { transform_ = transform; }
    const glm::mat4 &getTransform() const { return transform_; }

private:
    GLuint vao_;
    GLuint vboVertices_;
    GLuint vboIndices_;
    GLuint shaderProgram_;
    std::vector<glm::vec3> vertices_;
    std::vector<unsigned short> indices_;
    std::vector<std::vector<unsigned short>> triangles_;
    glm::mat4 transform_;
    Mesh *parent_;
    std::vector<Mesh *> children_;
    // GLuint texture_;
    // glm::vec2 color_;

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
};
