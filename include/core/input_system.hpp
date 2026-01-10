#ifndef __INPUT_SYSTEM_H
#define __INPUT_SYSTEM_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputSystem
{
private:
    bool pressedKeys[1024] = {0};
    bool touchedKeys[1024] = {0};
    GLfloat lastMouseX, lastMouseY;
    GLfloat xOffset, yOffset;
    bool cursorDisabled = true;

public:
    void handleInput(int key, int scancode, int action, int mode);
    void handleInput(double xpos, double ypos);
    InputSystem(GLFWwindow *window);
    bool isKeyPressed(int key);
    bool isKeyTouched(int key);
    bool isCursorDisabled();
    void setCursorDisabledState(bool state);
    glm::vec2 getMousePosition();
    glm::vec2 getMouseOffset();
};

#endif