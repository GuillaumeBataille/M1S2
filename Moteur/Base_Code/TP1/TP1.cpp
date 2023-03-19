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
#include "../TP1/Sphere.cpp"
#include "../TP1/Mesh.cpp"
#include "../TP1/Plane.cpp"
#include "../TP1/Cube.cpp"
#include "../TP1/camera.cpp"

#include <vector>
#include <string>
#include <glm/glm.hpp>

int main(void)
{

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

    //---------------------------SCENE CONTENT---------------------------//
    // Creating new GameObjects
    // Note pour le dernier argument
    // Pour sphere : Rien (0) = Couleur de la sphere - 1 = Texsoleil - 2 = TexTerre - 3 = TexMoon
    // Pour Cube : Rien(0) = Couleur du cube -  1 = TexSkybox - 2 = TexMinecraftDiamond
    GameObject *Root = new GameObject();
    GameObject *Plane = new GO_Plane(50, 100); // Resolution , Taille du plan
    GameObject *Sphere1 = new GO_Sphere(20, 2, 1);
    GameObject *Sphere2 = new GO_Sphere(20, 1, 2);
    GameObject *Sphere3 = new GO_Sphere(20, 1, 3);
    GameObject *Cube1 = new GO_Cube(100, 1);
    GameObject *CubetoSlide = new GO_Cube(3, 2);
    // Binding the Hierarchy
    Root->addChild(Plane);
    Root->addChild(Sphere1);
    Root->addChild(CubetoSlide);
    Sphere1->addChild(Sphere2);
    Sphere2->addChild(Sphere3);
    Root->addChild(Cube1);
    // Apply the texture to all the program
    Root->GO_Texture();

    // Set of transformation that will generate a static scene
    Plane->setColor(glm::vec4(1., 0.2, 0.2, 1));
    // Root->scale(glm::vec3(0.5));
    Sphere1->translate(glm::vec3(0., 10, 0.));
    Sphere2->scale(glm::vec3(0.8));
    Sphere2->translate(glm::vec3(0., 0, -10.));
    Sphere3->scale(glm::vec3(0.5));
    Sphere3->translate(glm::vec3(0., 0., -5.));
    CubetoSlide->translate(glm::vec3(1));

    //-------------------------CAMERA-----------------------------//

    // Background color
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test - Permet de faire une verification sur le visible/caché pour opti le rendu
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    // Dessine un pixel/fragment uniquement si c'est le plus proche de la cam
    // glDepthFunc(GL_LESS); Less de base
    glDepthFunc(GL_LEQUAL); // Mais on utilise une skybox donc LEQUAL
    //  For speed computation
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

        // Envoyer V et P au shader
        camera.sendToShader(shaderProgram_);

        // INPUT
        processInput(window, Plane);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Ani Animated stuff
        Sphere1->rotate((0.1 * deltaTime), glm::vec3(0., 1, 0.)); // Soleil rotate
        Sphere2->rotate((0.4 * deltaTime), glm::vec3(0., 1, 0.)); // Terre rotate

        CubetoSlide->translate(glm::vec3(0., 0., deltaTime)); // Translate du diamond bloc pour le TP

        //-------- Compute crado pour l'instant pour que le cube slide sur le terrain heightmapped
        glm::vec3 cubepos = CubetoSlide->getPos(); // Recupère la position courante du cube (bas gauche)
        glm::vec2 uv = calculateUV(cubepos, Plane->vertices_[0], Plane->vertices_[Plane->vertices_.size() - 1]);
        // Appliquer les uv dans la heightmap pour retrouver la hauteur de la ou est le cube
        float height = getGrayLevel(data, width, heights, uv[0], uv[1]);
        CubetoSlide->translate(glm::vec3(0., height - cubepos[1], 0.)); // Le translate pour recalibrer l'objet en y
        // DRAW SCENE///
        Root->draw(); //
        //------------//

        glfwSwapBuffers(window);

        //--------- Gestion du mode immersif avec souris pour bouger la cam
        // Récupérer la position actuelle de la souris
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // Définir la fonction de rappel pour la position de la souris
        glfwSetCursorPosCallback(window, mouse_callback); // Defini dans input
        // Ajout de la fonction de rappel à la fenêtre GLFW
        glfwSetScrollCallback(window, scroll_callback); // Defini dans input
        // Obtention des matrices de vue et de projection
        //-------------

        // Traiter les événements de la fenêtre
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteProgram(shaderProgram_);
    Root->DeleteBuffers();
    //  Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}
