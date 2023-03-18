#include "../TP1/function.cpp"

Mesh *generateS1()
{

    // Set rotation and orbit speeds
    float sunRotationSpeed = 0.1f;
    float earthRotationSpeed = 0.6f;

    // Create node of my scenegraph
    // Mesh *scene1 = new Mesh("");
    Mesh *sun = new Mesh("../mesh/off/sphere.off");
    Mesh *earth = new Mesh("../mesh/off/sphere.off");
    Mesh *moon = new Mesh("../mesh/off/sphere.off");

    // Bind my scenegraph together
    // scene1->addChild(sun);
    sun->addChild(earth);
    earth->addChild(moon);

    // Set sun transform
    sun->translate(glm::vec3(0., 1, 0.));
    sun->scale(glm::vec3(0.2));
    sun->rotate(sunRotationSpeed * glfwGetTime(), glm::vec3(0., 1, 0.));

    // Set earth transform
    earth->translate(glm::vec3(-3, 0, 0));
    earth->scale(glm::vec3(0.5));
    earth->rotate(earthRotationSpeed * glfwGetTime(), glm::vec3(0., 1, 0.));
    earth->rotate(glm::radians(23.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Inclinaison terre

    // Set moon transform
    moon->translate(glm::vec3(2, 0, 0));
    moon->scale(glm::vec3(0.2));
    moon->rotate(glm::radians(6.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Inclinaison lune

    // return scene1;
    return sun;
}
