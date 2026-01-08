#ifndef __CAMERA_H
#define __CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
private:
    glm::mat4 view;
    glm::mat4 projection;
    unsigned int windowWidth;
    unsigned int windowHeight;

public:
    glm::vec3 position;
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

#endif