#include <glm/glm.hpp>

#include <core/input_system.hpp>

InputSystem::InputSystem(GLFWwindow *window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    double firstMouseX, firstMouseY;
    glfwGetCursorPos(window, &firstMouseX, &firstMouseY);
    lastMouseX = firstMouseX;
    lastMouseY = firstMouseY;
}
void InputSystem::handleInput(int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        pressedKeys[key] = true;
        touchedKeys[key] = true;
    }
    else if (action == GLFW_RELEASE)
        pressedKeys[key] = false;
}

void InputSystem::handleMouseButton(int button, int action, int mods)
{
    if (button < 0 || button >= 8)
    {
        return;
    }

    if (action == GLFW_PRESS)
    {
        pressedMouseButtons[button] = true;
        touchedMouseButtons[button] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        pressedMouseButtons[button] = false;
    }
}

void InputSystem::handleInput(double xpos, double ypos)
{
    xOffset = xpos - lastMouseX;
    yOffset = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;
}

glm::vec2 InputSystem::getMousePosition()
{
    return glm::vec2(lastMouseX, lastMouseY);
}

glm::vec2 InputSystem::getMouseOffset()
{
    glm::vec2 cur_offset = glm::vec2(xOffset, yOffset);
    xOffset = 0.0f;
    yOffset = 0.0f;
    return cur_offset;
}

bool InputSystem::isKeyPressed(int key)
{
    return pressedKeys[key];
}

bool InputSystem::isKeyTouched(int key)
{
    if (touchedKeys[key])
    {
        touchedKeys[key] = false;
        return true;
    }
    return false;
}

bool InputSystem::isMouseButtonPressed(int button)
{
    if (button < 0 || button >= 8)
    {
        return false;
    }
    return pressedMouseButtons[button];
}

bool InputSystem::isMouseButtonTouched(int button)
{
    if (button < 0 || button >= 8)
    {
        return false;
    }
    if (touchedMouseButtons[button])
    {
        touchedMouseButtons[button] = false;
        return true;
    }
    return false;
}

bool InputSystem::isCursorDisabled()
{
    return cursorDisabled;
}

void InputSystem::setCursorDisabledState(bool state)
{
    cursorDisabled = state;
}

void InputSystem::resetMouseDelta(GLFWwindow *window)
{
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    lastMouseX = mouse_x;
    lastMouseY = mouse_y;
    xOffset = 0.0f;
    yOffset = 0.0f;
}
