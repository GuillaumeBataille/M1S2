// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

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

#define RESOLUTION 16
#define SIZE 2
#define offset 1

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 10000;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;

glm::mat4 mat_t = glm::mat4(1); // Ma matrice de transformation initialisé a identité
glm::mat4 mat_v = glm::mat4(1); // Ma matrice de vue initialisé a identité
glm::mat4 mat_p = glm::mat4(1); // Ma matrice de projection initialisé a identité

/*******************************************************************************/
std::vector<unsigned short> indices; //Triangles concaténés dans une liste
std::vector<std::vector<unsigned short>> triangles;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec2> uv;

void initPlane(std::vector<unsigned short> &indices, std::vector<std::vector<unsigned short>> &triangles, std::vector<glm::vec3> &indexed_vertices, int resolution, int size)
{
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int nbVertices = resolution * resolution; // Nombre de sommet au total
    std::cout << "nombre de sommet a dessiner :" << nbVertices << std::endl;
    float pas = size / (float)resolution;
    float x = 0, y = 0;

    for (int i = 0; i < resolution + 1; i++)
    {
        for (int j = 0; j < resolution + 1; j++)
        {
            std::cout << "valeur courante i : " << i << " et j: " << j << std::endl;
            x = j * pas;
            y = i * pas;
            std::cout << "vecteur courant = (" << x << "," << y << ",0) et on le recupère dans index_vertices" << std::endl;
            indexed_vertices.push_back(glm::vec3(x - offset, y - offset, 0));
            uv.push_back(glm::vec2(x - offset, y - offset));
        }
    }

    int cpt = 0;
    std::cout << "-----index vertices size " << indexed_vertices.size() << std::endl;
    for (glm::vec3 vert : indexed_vertices)
    {
        std::cout << "index vercices courant " << cpt << " : " << vert[0] << "," << vert[1] << "," << vert[2] << std::endl;
        std::cout << "uv du vertice courant " << cpt << " : " << uv[cpt][0] << "," << uv[cpt][1] << std::endl;

        cpt++;
    }
    for (int i = 0; i < resolution; i++) //hauteur
    {
        for (int j = 0; j < resolution; j++) //largeur
        {
            unsigned short bottomLeft = j + i * (resolution + 1);
            unsigned short bottomRight = bottomLeft + 1;
            unsigned short topLeft = bottomLeft + (resolution + 1);
            unsigned short topRight = topLeft + 1;

            triangles.push_back({bottomLeft, topLeft, bottomRight});
            triangles.push_back({topRight, topLeft, bottomRight});
        }
    }
    cpt = 0;
    std::cout << "------index triangles " << indexed_vertices.size() << std::endl;
    for (std::vector<unsigned short> tri : triangles)
    {
        std::cout << "index triangle courant " << cpt << " : " << tri[0] << "," << tri[1] << "," << tri[2] << std::endl;
        cpt++;
    }
    for (unsigned short i = 0; i < triangles.size(); i++) // Recupère les id des sommets des triangles from "triangles" in "indices"
    {
        indices.push_back(triangles[i][0]);
        indices.push_back(triangles[i][1]);
        indices.push_back(triangles[i][2]);
    }
}

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
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short>> triangles;
    std::vector<glm::vec3> indexed_vertices;

    //Chargement du fichier de maillage // OLD
    std::string filename("suzanne.off");
    //loadOFF(filename, indexed_vertices, indices, triangles);
    initPlane(indices, triangles, indexed_vertices, RESOLUTION, SIZE); // Initiation du plan

    //---------------------Loading grass.bmp --------------------//
    GLuint texture1 = loadBMP_custom("../textures/grass.bmp");
    GLuint samplerUniformLocation = glGetUniformLocation(programID, "TextureSampler");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(samplerUniformLocation, 1);

    //-------------------Loading

    // Load it into a VBO
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate uv VB0
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    do
    {
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

        /*****************TODO***********************/
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

        /*********************** *****************/
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            0,        // attribute
            2,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            0,        // attribute
            2,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glDisableVertexAttribArray(0);

        glEnableVertexAttribArray(0);
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0          // element array buffer offset
        );

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
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

    //------------------------TRANSLATION --------------------------------//

    //UP

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        //camera_position -= cameraSpeed * camera_target;
        mat_t = glm::translate(mat_t, glm::vec3(0., -0.005, 0.));
    }
    //DOWN

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        //camera_position -= cameraSpeed * camera_target;
        mat_t = glm::translate(mat_t, glm::vec3(0., +0.005, 0.));
    }
    //RIGHT

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        //camera_position -= cameraSpeed * camera_target;
        mat_t = glm::translate(mat_t, glm::vec3(-0.005, 0, 0.));
    }
    //LEFT

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        //camera_position -= cameraSpeed * camera_target;
        mat_t = glm::translate(mat_t, glm::vec3(+0.005, 0, 0.));
    }

    //TODO add translations
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
