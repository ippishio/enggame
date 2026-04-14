#ifndef __ENGINE_H
#define __ENGINE_H

#include <graphics/renderer.hpp>
#include <core/input_system.hpp>
#include <physics/physics_engine.hpp>

class Engine
{
private:
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputSystem> input_system;
    std::unique_ptr<PhysicsEngine> physics_engine;

public:
    Engine();
    void tick(Scene &scene);
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    float cameraSpeed = 5.0f;
    GLfloat cameraSensitivity = 0.05f;
    Renderer &getRenderer();
    InputSystem &getInputSystem();
    PhysicsEngine &getPhysicsEngine();
};

#endif
