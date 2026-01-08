#ifndef __RENDERER_H
#define __RENDERER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <graphics/camera.hpp>
#include <graphics/shader_program.hpp>

class Renderer
{
private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
    void handleFramebufferSizeChange(int width, int height);
    void handleInput(int key, int scancode, int action, int mode);
    unsigned int projectionLocId;
    unsigned int viewLocId;
    bool pressedKeys[1024] = {0};

public:
    GLFWwindow *window;
    Renderer(int window_x = 800, int window_y = 600);
    ~Renderer();
    void tick();
    void updateCamera();
    unsigned int windowWidth;
    unsigned int windowHeight;
    Camera *camera = nullptr;
    ShaderProgram *shader = nullptr;
    float cameraSpeed = 5.0f;
    GLfloat lastFrame = 0.0f;
    GLfloat deltaTime = 0.0f;
    
};

#endif
