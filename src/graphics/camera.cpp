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

void cameraFreeFly(Camera &camera, InputSystem &input, float camera_sensitivity,
                   float camera_speed)
{
    glm::vec2 offset = input.getMouseOffset();
    offset *= camera_sensitivity;
    camera.rotation.y += offset.x;
    camera.rotation.x += offset.y;
    if (camera.rotation.x > 89.0f)
        camera.rotation.x = 89.0f;
    if (camera.rotation.x < -89.0f)
        camera.rotation.x = -89.0f;
    if (input.isKeyPressed(GLFW_KEY_W))
        camera.position += camera_speed * camera.front;
    if (input.isKeyPressed(GLFW_KEY_S))
        camera.position -= camera_speed * camera.front;
    if (input.isKeyPressed(GLFW_KEY_A))
        camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * camera_speed;
    if (input.isKeyPressed(GLFW_KEY_D))
        camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * camera_speed;
}