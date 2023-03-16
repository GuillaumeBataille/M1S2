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
#include <TP1/mesh.cpp>

//
#define SIZE 2
#define NBR_SCENE 3
// settings
const unsigned int SCR_WIDTH = 10000;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position = glm::vec3(0.f, 3.f, -1.5f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.f, 0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.f, 0.0f);

// timing

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// rotation
float angle = 0.;
float zoom = 1.;

int resolution = 10;
bool randomheight = false;
float cameraOrbitSpeed; // adjust this value to change the orbit speed
unsigned int current_scene = 0;

std::vector<Mesh *> scenes;

glm::mat4 mat_t = glm::mat4(1); // Ma matrice de transformation initialisé a identité
glm::mat4 mat_v = glm::mat4(1); // Ma matrice de vue initialisé a identité
glm::mat4 mat_p = glm::mat4(1); // Ma matrice de projection initialisé a identité

glm::mat4 mat_t_m = glm::mat4(1.f);
/*******************************************************************************/
std::vector<unsigned short> indices; // Triangles concaténés dans une liste
std::vector<std::vector<unsigned short>> triangles;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec3> normals;
std::vector<glm::vec2> uv;

#endif