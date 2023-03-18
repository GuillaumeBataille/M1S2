#ifndef CAMERA_CPP
#define CAMERA_CPP
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
// #include "common/my_texture.cpp

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : position(position), worldUp(up), yaw(yaw), pitch(pitch), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
    {
        zoom = ZOOM;
        movementSpeed = SPEED;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(int direction, float deltaTime)
    {
        // std::cout << "DIRECTION :" << direction << std::endl;
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        // std::cout << "xoffset " << xoffset << std::endl;
        // xoffset *= mouseSensitivity;
        // yoffset *= mouseSensitivity;
        // std::cout << "xoffset " << xoffset << std::endl;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();

        // Debug output
        // std::cout << "Offset x " << yoffset << "Camera position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
        // std::cout << "Camera yaw: " << yaw << ", pitch: " << pitch << std::endl;
    }

    void processMouseScroll(float yoffset)
    {
        if (zoom >= 1.0f && zoom <= 45.0f)
            zoom -= yoffset;
        if (zoom <= 1.0f)
            zoom = 1.0f;
        if (zoom >= 45.0f)
            zoom = 45.0f;
    }
    void sendToShader(GLuint shaderProgram)
    {
        glm::mat4 view = getViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "matv"), 1, false, &(view[0][0]));
        glm::mat4 projection = glm::perspective(glm::radians(zoom), (float)4 / (float)3, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "matp"), 1, false, &(projection[0][0]));
    }

private:
    glm::vec3 position;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    void updateCameraVectors()
    {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;
    const int FORWARD = GLFW_KEY_W;
    const int LEFT = GLFW_KEY_A;
    const int BACKWARD = GLFW_KEY_S;
    const int RIGHT = GLFW_KEY_D;
};
#endif