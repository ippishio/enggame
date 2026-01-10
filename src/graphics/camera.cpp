#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <graphics/camera.hpp>

Camera::Camera(unsigned int windowWidth, unsigned int windowHeight, glm::vec3 cameraPos,
               glm::vec3 cameraFront, glm::vec3 cameraUp)
{
    setPosition(cameraPos);
    front = cameraFront;
    up = cameraUp;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    updateWindow(windowWidth, windowHeight);
}

void Camera::updateWindow(unsigned int windowWidth, unsigned int windowHeight)
{
    std::cout << "new asecct ratio: " << (float)windowWidth / (float)windowHeight << std::endl;
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
    if (input.isCursorDisabled())
    {
        glm::vec3 offset = glm::vec3(input.getMouseOffset(), 0.0f);
        offset *= camera_sensitivity;
        GLfloat _x = offset.x;
        offset.x = offset.y;
        offset.y = _x;
        glm::vec3 new_rotation = camera.rotation + offset;
        if (new_rotation.x > 89.0f)
            new_rotation.x = 89.0f;
        if (new_rotation.x < -89.0f)
            new_rotation.x = -89.0f;
        camera.setRotation(new_rotation);

        if (input.isKeyPressed(GLFW_KEY_W))
            camera.setPosition(camera.position + camera_speed * camera.front);
        if (input.isKeyPressed(GLFW_KEY_S))
            camera.setPosition(camera.position - camera_speed * camera.front);
        if (input.isKeyPressed(GLFW_KEY_A))
            camera.setPosition(camera.position - glm::normalize(glm::cross(camera.front, camera.up)) * camera_speed);
        if (input.isKeyPressed(GLFW_KEY_D))
            camera.setPosition(camera.position + glm::normalize(glm::cross(camera.front, camera.up)) * camera_speed);
        if (input.isKeyPressed(GLFW_KEY_Q))
            camera.setPosition(camera.position + camera_speed * camera.up);
        if (input.isKeyPressed(GLFW_KEY_E))
            camera.setPosition(camera.position - camera_speed * camera.up);
        if (input.isKeyPressed(GLFW_KEY_F))
            camera.lookAt(glm::vec3(0.0f));
    }
}