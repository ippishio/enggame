#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <graphics/camera.hpp>

Camera::Camera(unsigned int windowWidth, unsigned int windowHeight, glm::vec3 cameraPos,
               glm::vec3 cameraFront, glm::vec3 cameraUp)
{
    position = cameraPos;
    front = cameraFront;
    up = cameraUp;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    updateWindow(windowWidth, windowHeight);
}

void Camera::updateWindow(unsigned int windowWidth, unsigned int windowHeight)
{
    this->windowHeight = windowHeight;
    this->windowWidth = windowWidth;
    projection = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
}

glm::mat4 Camera::getViewMatrix()
{
    view = glm::lookAt(position, position + front, up);
    return view;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return projection;
}