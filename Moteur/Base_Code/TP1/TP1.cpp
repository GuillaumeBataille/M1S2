// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cstdlib>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include "../common/shader.hpp"
#include "../common/vboindexer.hpp"

#include "../common/texture.hpp"
#include "../TP1/input.cpp"
#include "../TP1/function.cpp"
#include "../TP1/scenes.cpp"
#include "../TP1/Sphere.cpp"
#include "../TP1/Mesh.cpp"
#include "../TP1/Plane.cpp"
#include "../TP1/camera.cpp"

#include <vector>
#include <string>
#include <glm/glm.hpp>

double lastX = 0.0, lastY = 0.0; // Position précédente de la souris
bool firstMouse = true;          // Indicateur pour la première utilisation de la souris
Camera camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
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

int main(void)
{
    //

    // Root->GenerateBuffers();
    //  Root->DeleteBuffers();
    //   Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "MOTEUR DE JEU", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Mode d'input
    //  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);         // Mode keyboard + mouse not use classique
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Mode capture de la souris

    // Bind et Activation du shader global au programme
    shaderProgram_ = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");
    glUseProgram(shaderProgram_);
    GLuint viewLoc = glGetUniformLocation(shaderProgram_, "Viewss");
    GLuint projLoc = glGetUniformLocation(shaderProgram_, "Projects");
    //---------------------------SCENE CONTENT---------------------------//
    // Creating new GameObjects
    GameObject *Root = new GameObject();
    GameObject *Plane = new GO_Plane(50, 100);
    GameObject *Sphere1 = new GO_Sphere(20, 2, 1);
    GameObject *Sphere2 = new GO_Sphere(20, 1, 2);
    GameObject *Sphere3 = new GO_Sphere(20, 1, 3);
    // Binding the Hierarchy
    Root->addChild(Plane);
    Root->addChild(Sphere1);
    Sphere1->addChild(Sphere2);
    Sphere2->addChild(Sphere3);
    // Apply the texture to all the program
    Root->GO_Texture();

    // Set of transformation that will generate a static scene
    Plane->setColor(glm::vec4(1., 0.2, 0.2, 1));
    Plane->translate(glm::vec3(0.2));
    Root->scale(glm::vec3(0.5));
    Sphere1->translate(glm::vec3(0., 10, 0.));
    Sphere2->scale(glm::vec3(0.8));
    Sphere2->translate(glm::vec3(0., 0, -10.));
    Sphere3->scale(glm::vec3(0.5));
    Sphere3->translate(glm::vec3(0., 0., -5.));

    // glfwPollEvents(); // Créer une pool qui va gérer les events Glfw comme les inputs
    //  glfwSetCursorPos(window, 1024 / 2, 768 / 2); // Mets a souris a une position particulière (pas use pour l'instant)

    //-------------------------CAMERA-----------------------------//

    // Background color
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test - Permet de faire une verification sur le visible/caché pour opti le rendu
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    // Dessine un pixel/fragment uniquement si c'est le plus proche de la cam
    glDepthFunc(GL_LESS);

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    double accumulator = 0.;
    char titre[50]; // Pour l'affichage au dessus
    do
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        nbFrames++;
        accumulator += deltaTime;

        if (accumulator >= 1) // A chaque seconde
        {
            float fps = nbFrames / accumulator;
            sprintf(titre, "MOTEUR DE JEU - (%.1f FPS)", fps); // Formatage du titre avec les FPS
            glfwSetWindowTitle(window, titre);                 // Mise à jour du titre de la fenêtre
            nbFrames = 0;
            accumulator = 0.;
        }

        // INPUT
        camera.sendToShader(shaderProgram_);
        if (glfwGetKey(window, FORWARD) == GLFW_PRESS)
            camera.processKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, BACKWARD) == GLFW_PRESS)
            camera.processKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, LEFT) == GLFW_PRESS)
            camera.processKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, RIGHT) == GLFW_PRESS)
            camera.processKeyboard(RIGHT, deltaTime);

        processInput(window, camera);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Ani Animated stuff
        Sphere1->rotate((0.1 * deltaTime), glm::vec3(0., 1, 0.));
        Sphere2->rotate((0.4 * deltaTime), glm::vec3(0., 1, 0.));

        // Draw a scene
        Root->draw();

        glfwSwapBuffers(window);
        // Récupérer la position actuelle de la souris
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Définir la fonction de rappel pour la position de la souris
        glfwSetCursorPosCallback(window, mouse_callback);
        // Ajout de la fonction de rappel à la fenêtre GLFW
        glfwSetScrollCallback(window, scroll_callback);
        // Obtention des matrices de vue et de projection

        // Traiter les événements de la fenêtre
        glfwPollEvents();

        // mat_v = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteProgram(shaderProgram_);
    Root->DeleteBuffers();
    //  Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
