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


#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>
#include <TP1/input.cpp>
#include "TP1/function.cpp"

int main(void)
{
    // Initialise GLFW
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
    window = glfwCreateWindow(1024, 768, "TP1 - GLFW", NULL, NULL);
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
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Set up your mouse callback function


    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
        GLuint vertexbuffer;
        GLuint uvbuffer;
        GLuint elementbuffer;
        GLuint normalbuffer;
        GLuint tangentbuffer;
        GLuint bitangentbuffer;

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");
    glUseProgram(programID);

    //---------------------Loading textures and heightmap --------------------//
    
        glActiveTexture(GL_TEXTURE0);
    
    loadBMP_custom("../textures/HM1.bmp");
    //loadBMP_custom("../textures/HM2.bmp");
    //loadBMP_custom("../textures/HM3.bmp");
    glUniform1i(glGetUniformLocation(programID, "texture0"), 0);
    
    glActiveTexture(GL_TEXTURE1);
    loadBMP_custom("../textures/grass.bmp");
    glUniform1i(glGetUniformLocation(programID, "texture1"), 1);
    
    glActiveTexture(GL_TEXTURE2);
    loadBMP_custom("../textures/rock.bmp");
    glUniform1i(glGetUniformLocation(programID, "texture2"), 2);
    
    glActiveTexture(GL_TEXTURE3);
    loadBMP_custom("../textures/snowrock.bmp");
    glUniform1i(glGetUniformLocation(programID, "texture3"), 3);
    



    //Créer mes buffers 
    
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &uvbuffer);
    glGenBuffers(1, &elementbuffer);
    glGenBuffers(1,&normalbuffer);

    // Get a handle for our "LightPosition" uniform
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
   
    do
    {
        //std::cout<<"current resolution : "<< resolution << std::endl;
        //glfwSetCursorPosCallback(window, mouseCallback);
        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute le necessaire pour avoir de quoi charger les buffers
        initPlane(indices, triangles, indexed_vertices, uv,resolution, SIZE, randomheight); // Initiation du plan
        computeNormals(indexed_vertices,indices,normals);
        computeUV(uv,resolution);
        targetCameraPlan(indexed_vertices);

        //Envoi des matrices MVP (ici TVP)
        // Model matrix : an identity matrix (model will be at the origin) then change
        GLuint id_t = glGetUniformLocation(programID, "transform_mat");
        glUniformMatrix4fv(id_t, 1, false, &mat_t[0][0]);
        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up
        mat_v = glm::lookAt(camera_position, camera_target, camera_up);
        GLuint id_v = glGetUniformLocation(programID, "view_mat");
        glUniformMatrix4fv(id_v, 1, false, &mat_v[0][0]);
        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        mat_p = glm::perspective(glm::radians(45.0f), (float)4 / (float)3, 0.1f, 100.0f);
        GLuint id_p = glGetUniformLocation(programID, "project_mat");
        glUniformMatrix4fv(id_p, 1, false, &mat_p[0][0]);


        // On bind, charge et envoie le contenu du vertexbuffer dans le layout 0 
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(
            0,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );
        glEnableVertexAttribArray(0);//Activer le layout

        //On bind, charge et envoie le contenu de l'uvbuffer (pour les textures/heightmap) dans le layout 1
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
        glVertexAttribPointer(
            1,        // attribute (layout)
            2,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );
        glEnableVertexAttribArray(1); //Activer le layout

        //On bind, charge et envoie le contenu du normalbuffer dans le layout 2 
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(
            2,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );
        glEnableVertexAttribArray(2);//Activer le layout


        //Bind puis Envoi des indices des triangles pour draw les elements
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
        // Draw the triangles !
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0          // element array buffer offset
        );

        //Desactiver les layouts après avoir dessiner
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}



