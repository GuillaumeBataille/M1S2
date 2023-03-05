#ifndef INPUT_CPP
#define INPUT_CPP

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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    bool keypressed =false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //-------------------Camera Zoom in/out ----------------------------//
    float cameraSpeed = 2.5 * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm::vec3 front = glm::normalize(camera_target - camera_position);
        camera_position += cameraSpeed * front;
        camera_target += cameraSpeed * front;
        mat_v = glm::lookAt(camera_position, camera_target, camera_up);
        mat_p = glm::perspective(glm::radians(45.0f), (float)4 / (float)3, 0.1f, 100.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm::vec3 front = glm::normalize(camera_target - camera_position);
        camera_position -= cameraSpeed * front;
        camera_target -= cameraSpeed * front;
        mat_v = glm::lookAt(camera_position, camera_target, camera_up);
        mat_p = glm::perspective(glm::radians(45.0f), (float)4 / (float)3, 0.1f, 100.0f);
    }

    //------------------------CAMERA MOVEMENTS--------------------------------//

// UP
if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    camera_position += cameraSpeed * camera_up;
}

// DOWN
if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    camera_position -= cameraSpeed * camera_up;
}

// RIGHT
if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    glm::vec3 rightVector = glm::normalize(glm::cross(camera_up, camera_target));
    camera_position += cameraSpeed * rightVector;
}

// LEFT
if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    glm::vec3 leftVector = -glm::normalize(glm::cross(camera_up, camera_target));
    camera_position += cameraSpeed * leftVector;
}


    //----------------------- CAMERA ORBIT SPEED ---------------------------//
if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
{
    cameraOrbitSpeed+=0.0001;
}

if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
{
    cameraOrbitSpeed-=0.0001;
}


//-------------------Camera Orbit ----------------------------//
static bool isOrbiting = false;

if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
{
    isOrbiting = true; // toggle the orbiting state
    cameraOrbitSpeed=0.001;
}
//std::cout<<"Orbit value : "<< isOrbiting<<std::endl;

if (isOrbiting)
{
    camera_target = indexed_vertices[0]+indexed_vertices[indexed_vertices.size()-1] - camera_position ;
    float radius = glm::distance(camera_position, camera_target);
    float horizontalAngle = atan2(camera_position.z - camera_target.z, camera_position.x - camera_target.x);
    float verticalAngle = atan2(camera_position.y - camera_target.y+5, radius/1.1);

    horizontalAngle -= cameraOrbitSpeed;

    float newCameraPosX = camera_target.x + radius * cos(verticalAngle) * cos(horizontalAngle);
    float newCameraPosZ = camera_target.z + radius * cos(verticalAngle) * sin(horizontalAngle);
    float newCameraPosY = camera_target.y + radius * sin(verticalAngle);
    camera_position = glm::vec3(newCameraPosX, newCameraPosY, newCameraPosZ);
}

if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
{
    isOrbiting = false; // toggle the orbiting state
    camera_position = glm::vec3(0.f, 3.f, -1.5f);
    camera_target = glm::vec3(0.0f, 0.f, 0.0f);
}

//-------------------RESOLUTION OF THE PLANE -------------//
if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS )
{
    resolution++;
}
if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
{
    if(resolution > 1)
    resolution--;
}

//---------------- VIEW MODE -----------------------//
if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
{
 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
{
 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
#endif