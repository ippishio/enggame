#ifndef __INPUT_SYSTEM_H
#define __INPUT_SYSTEM_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputSystem
{
private:
    bool pressedKeys[1024] = {0};
    bool touchedKeys[1024] = {0};
    bool pressedMouseButtons[8] = {0};
    bool touchedMouseButtons[8] = {0};
    GLfloat lastMouseX, lastMouseY;
    GLfloat xOffset, yOffset;
    bool cursorDisabled = true;

public:
    void handleInput(int key, int scancode, int action, int mode);
    void handleMouseButton(int button, int action, int mods);
    void handleInput(double xpos, double ypos);
    InputSystem(GLFWwindow *window);
    bool isKeyPressed(int key);
    bool isKeyTouched(int key);
    bool isMouseButtonPressed(int button);
    bool isMouseButtonTouched(int button);
    bool isCursorDisabled();
    void setCursorDisabledState(bool state);
    glm::vec2 getMousePosition();
    glm::vec2 getMouseOffset();
    void resetMouseDelta(GLFWwindow *window);
};

#endif
