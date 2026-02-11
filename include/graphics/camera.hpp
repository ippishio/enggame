#ifndef __CAMERA_H
#define __CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <core/transform.hpp>
#include <core/input_system.hpp>

class Camera : public Transform
{
private:
    glm::mat4 view;
    glm::mat4 projection;

public:
    glm::vec3 front;
    glm::vec3 up;
    Camera(unsigned int windowWidth, unsigned int windowHeight,
           glm::vec3 cameraPos,
           glm::vec3 cameraFront,
           glm::vec3 cameraUp);
    void updateWindow(unsigned int windowWidth, unsigned int windowHeight);
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
};

void cameraFreeFly(Camera &camera, InputSystem &input, float camera_sensitivity = 0.05f, float camera_speed = 5.0f);
#endif