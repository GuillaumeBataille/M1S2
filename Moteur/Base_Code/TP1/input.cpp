#ifndef INPUT_CPP
#define INPUT_CPP

#include "../TP1/variable.cpp"
#include "../TP1/camera.cpp"
#include "../TP1/Plane.cpp"
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

//----------------------------ZOOM et MOUSETRACKING ----------------------------------//
double lastX = 0.0, lastY = 0.0; // Position précédente de la souris
bool firstMouse = true;          // Indicateur pour la première utilisation de la souris
float rotatevalue = 0.f;
static bool isOrbiting;
static bool isPlayer;
const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

int offset = 3;

// Fonction de rappel pour la position de la souris
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    // Si c'est la première fois que la souris est utilisée, initialiser les valeurs de position précédente
    if (firstMouse)
    {
        // std::cout << "Mouse init - Xpos : " << xpos << " and Ypos : " << ypos << std::endl;
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculer le déplacement de la souris
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // La distance y est inversée
    // Mettre à jour la position de la caméra en fonction du déplacement de la souris
    // Ici, on ne fait que modifier l'angle de direction de la caméra, mais vous pouvez ajouter de la logique supplémentaire selon vos besoins
    const float sensitivity = 0.05f;
    // std::cout << "Mouse init - Xpos : " << xoffset * sensitivity << " and Ypos : " << yoffset * sensitivity << std::endl;
    if (!isOrbiting)
        camera.processMouseMovement(xoffset * sensitivity, yoffset * sensitivity);
    //  Réinitialiser la position précédente de la souris
    lastX = xpos;
    lastY = ypos;
}

// Définition de la fonction de rappel pour le scroll de la souris
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}
//---------------------------------------------------------------------------//

//-------------------------------Autre inputs--------------------------------//
void processInput(GLFWwindow *window, GameObject *scene)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //-------------------Camera Orbit ----------------------------//

    float distance = 20.0f;
    float horizontalAngle = 40.0f;
    float verticalAngle = 10.0f;
    rotatevalue += deltaTime;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        isOrbiting = true; // toggle the orbiting state
        cameraOrbitSpeed = 0.001;
    }
    // std::cout<<"Orbit value : "<< isOrbiting<<std::endl;
    // Variables pour la position de la caméra
    float cameraRadius = 50.0f;
    float cameraHeight = 5.0f;
    float cameraAngle = 0.0f;

    if (isOrbiting)
    {
        float radius = cameraRadius;
        glm::vec3 camera_position = camera.getPosition();
        glm::vec3 camera_target = camera.getFront();
        float horizontalAngle = atan2(camera_position.z - camera_target.z, camera_position.x - camera_target.x);
        float verticalAngle = atan2(camera_position.y - camera_target.y + 5, radius / 1.1);

        horizontalAngle -= cameraOrbitSpeed;

        float newCameraPosX = camera_target.x + radius * cos(verticalAngle) * cos(horizontalAngle);
        float newCameraPosZ = camera_target.z + radius * cos(verticalAngle) * sin(horizontalAngle);
        float newCameraPosY = camera_target.y + radius * sin(verticalAngle);
        camera.setPosition(glm::vec3(newCameraPosX, newCameraPosY, newCameraPosZ));
        camera.setFront(glm::normalize(glm::vec3(0.) - camera.getPosition()));
        camera.setUp(worldUp);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        isOrbiting = false; // toggle the orbiting state
        camera.setPosition(cam_pos_saved);
        camera.setFront(cam_front_saved);
        camera.updateCameraVectors();
    }

    //-------------------RESOLUTION OF THE PLANE -------------//
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        resolution++;
        static_cast<GO_Plane *>(scene)->rebuild(resolution);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        if (resolution > 1)
        {
            resolution--;
            static_cast<GO_Plane *>(scene)->rebuild(resolution);
        }
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
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        isOrbiting = false;
        isPlayer = true;

        glm::vec2 res = calculateUV(cam_pos_saved, scene->vertices_[0], scene->vertices_[scene->vertices_.size() - 1]);
        camera.setPosition(glm::vec3(0., offset + getGrayLevel(data, width, heights, res[0], res[1]), 0.));
    }
    if (glfwGetKey(window, FORWARD) == GLFW_PRESS)
    {
        camera.processKeyboard(FORWARD, deltaTime);
        if (isPlayer)
        {
            glm::vec3 campos = camera.getPosition();
            glm::vec2 res = calculateUV(campos, scene->vertices_[0], scene->vertices_[scene->vertices_.size() - 1]);
            camera.setPosition(glm::vec3(campos.x, offset + getGrayLevel(data, width, heights, res[0], res[1]), campos.z));
        }
    }

    if (glfwGetKey(window, BACKWARD) == GLFW_PRESS)
    {
        camera.processKeyboard(BACKWARD, deltaTime);
        if (isPlayer)
        {
            glm::vec3 campos = camera.getPosition();
            glm::vec2 res = calculateUV(campos, scene->vertices_[0], scene->vertices_[scene->vertices_.size() - 1]);
            camera.setPosition(glm::vec3(campos.x, offset + getGrayLevel(data, width, heights, res[0], res[1]), campos.z));
        }
    }
    if (glfwGetKey(window, LEFT) == GLFW_PRESS)
    {
        camera.processKeyboard(LEFT, deltaTime);
        if (isPlayer)
        {
            glm::vec3 campos = camera.getPosition();
            glm::vec2 res = calculateUV(campos, scene->vertices_[0], scene->vertices_[scene->vertices_.size() - 1]);
            camera.setPosition(glm::vec3(campos.x, offset + getGrayLevel(data, width, heights, res[0], res[1]), campos.z));
        }
    }
    if (glfwGetKey(window, RIGHT) == GLFW_PRESS)
    {
        camera.processKeyboard(RIGHT, deltaTime);
        if (isPlayer)
        {
            glm::vec3 campos = camera.getPosition();
            glm::vec2 res = calculateUV(campos, scene->vertices_[0], scene->vertices_[scene->vertices_.size() - 1]);
            camera.setPosition(glm::vec3(campos.x, offset + getGrayLevel(data, width, heights, res[0], res[1]), campos.z));
        }
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