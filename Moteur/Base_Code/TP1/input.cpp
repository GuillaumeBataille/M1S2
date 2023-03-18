#ifndef INPUT_CPP
#define INPUT_CPP

#include "../TP1/variable.cpp"
#include "../TP1/camera.cpp"
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void processInput(GLFWwindow *window, Camera camera)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //----------------------- CAMERA ORBIT SPEED ---------------------------//
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        cameraOrbitSpeed += 0.0001;
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        cameraOrbitSpeed -= 0.0001;
    }

    //-------------------Camera Orbit ----------------------------//
    static bool isOrbiting = false;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        isOrbiting = true; // toggle the orbiting state
        cameraOrbitSpeed = 0.001;
    }
    // std::cout<<"Orbit value : "<< isOrbiting<<std::endl;

    if (isOrbiting)
    {

        /*float radius = glm::distance(camera_position, camera_target);
        float horizontalAngle = atan2(camera_position.z - camera_target.z, camera_position.x - camera_target.x);
        float verticalAngle = atan2(camera_position.y - camera_target.y + 5, radius / 1.1);

        horizontalAngle -= cameraOrbitSpeed;

        float newCameraPosX = camera_target.x + radius * cos(verticalAngle) * cos(horizontalAngle);
        float newCameraPosZ = camera_target.z + radius * cos(verticalAngle) * sin(horizontalAngle);
        float newCameraPosY = camera_target.y + radius * sin(verticalAngle);
        camera_position = glm::vec3(newCameraPosX, newCameraPosY, newCameraPosZ);*/
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        isOrbiting = false; // toggle the orbiting state
        glm::vec3 camera_position = glm::vec3(0.f, 15.f, -5.f);
    }

    //-------------------RESOLUTION OF THE PLANE -------------//
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        resolution++;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        if (resolution > 1)
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