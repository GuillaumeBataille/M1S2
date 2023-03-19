#ifndef VARIABLE_CPP
#define VARIABLE_CPP

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp> // Inclure les fonctions d'intersection de GLM
// Include vector
#include <vector>
#include "../TP1/camera.cpp"
//
#define SIZE 20
#define NBR_SCENE 3
// settings
const unsigned int SCR_WIDTH = 10000;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -90.f);
glm::vec3 cam_pos_saved = camera.getPosition();
glm::vec3 cam_front_saved = camera.getFront();

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

const int FORWARD = GLFW_KEY_W;
const int LEFT = GLFW_KEY_A;
const int BACKWARD = GLFW_KEY_S;
const int RIGHT = GLFW_KEY_D;

// Pour recup les coordonnées de texture/heightmap dans le CPU
int width, heights, nbrChannel;
std::vector<unsigned char> data; // Stockage de la composante R de la heightmap
/*******************************************************************************/
// Surement useless atm mais faudrait verifier si le delete casse pas tout
std::vector<unsigned short> indices;
std::vector<std::vector<unsigned short>> triangles;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec3> normals;
std::vector<glm::vec2> uv;
/********************************************************************************/

GLuint shaderProgram_;

#endif