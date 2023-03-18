#ifndef VARIABLE_CPP
#define VARIABLE_CPP

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include vector
#include <vector>
#include "../TP1/mesh.cpp"

//
#define SIZE 20
#define NBR_SCENE 3
// settings
const unsigned int SCR_WIDTH = 10000;
const unsigned int SCR_HEIGHT = 600;

// camera
/*glm::vec3 camera_position = glm::vec3(0.f, 15.f, -5.f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.f, 0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.f, 0.0f);*/

// timing

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// rotation
float angle = 0.;
float zoom = 1.;

int resolution = 20;

bool randomheight = false;
float cameraOrbitSpeed; // adjust this value to change the orbit speed
unsigned int current_scene = 0;
unsigned int current_texture = 0;
std::vector<Mesh *> scenes;
const int FORWARD = GLFW_KEY_W;
const int LEFT = GLFW_KEY_A;
const int BACKWARD = GLFW_KEY_S;
const int RIGHT = GLFW_KEY_D;
/*glm::mat4 mat_t = glm::mat4(1); // Ma matrice de transformation initialisé a identité
glm::mat4 mat_v = glm::lookAt(camera_position, camera_target, camera_up);                   // Ma matrice de vue initialisé a identité
glm::mat4 mat_p = glm::perspective(glm::radians(45.0f), (float)4 / (float)3, 0.1f, 100.0f); // Ma matrice de projection initialisé a identité
glm::mat4 mat_t_m = glm::mat4(1.f);*/
/*******************************************************************************/
std::vector<unsigned short> indices; // Triangles concaténés dans une liste
std::vector<std::vector<unsigned short>> triangles;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec3> normals;
std::vector<glm::vec2> uv;

GLuint shaderProgram_;

#endif