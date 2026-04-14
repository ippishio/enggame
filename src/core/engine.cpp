#include <iostream>

#include <core/engine.hpp>
#include <core/input_system.hpp>
#include <core/scene.hpp>
#include <graphics/renderer.hpp>
#include <algorithm>

Engine::Engine()
{
    renderer.reset(new Renderer(800, 600));
    glfwSetWindowUserPointer(renderer->getWindow(), this);
    glfwSetFramebufferSizeCallback(renderer->getWindow(), framebufferSizeCallback);
    glfwSetKeyCallback(renderer->getWindow(), keyCallback);
    glfwSetMouseButtonCallback(renderer->getWindow(), mouseButtonCallback);
    glfwSetCursorPosCallback(renderer->getWindow(), mouseCallback);
    input_system.reset(new InputSystem(renderer->getWindow()));
    physics_engine.reset(new PhysicsEngine());
}

void Engine::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    Renderer &renderer = static_cast<Engine *>(glfwGetWindowUserPointer(window))->getRenderer();
    renderer.handleFramebufferSizeChange(width, height);
}

void Engine::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    InputSystem &input_system = static_cast<Engine *>(glfwGetWindowUserPointer(window))->getInputSystem();
    input_system.handleInput(key, scancode, action, mode);
}

void Engine::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    InputSystem &input_system = static_cast<Engine *>(glfwGetWindowUserPointer(window))->getInputSystem();
    input_system.handleMouseButton(button, action, mods);
}

void Engine::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    InputSystem &input_system = static_cast<Engine *>(glfwGetWindowUserPointer(window))->getInputSystem();
    input_system.handleInput(xpos, ypos);
}

void Engine::tick(Scene &scene)
{
    glfwPollEvents();
    if (input_system->isKeyTouched(GLFW_KEY_ESCAPE))
    {
        std::cout << "escape\n";
        if (input_system->isCursorDisabled())
        {
            glfwSetInputMode(renderer->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            input_system->setCursorDisabledState(false);
            input_system->resetMouseDelta(renderer->getWindow());
        }
        else
        {
            glfwSetInputMode(renderer->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            input_system->setCursorDisabledState(true);
            input_system->resetMouseDelta(renderer->getWindow());
        }
    }
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    deltaTime = std::clamp(deltaTime, 0.0f, 1.0f / 30.0f);
    physics_engine->tick(scene, deltaTime);
    renderer->tick();
}

Renderer &Engine::getRenderer()
{
    return *renderer;
}

InputSystem &Engine::getInputSystem()
{
    return *input_system;
}

PhysicsEngine &Engine::getPhysicsEngine()
{
    return *physics_engine;
}
