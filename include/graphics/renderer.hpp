#ifndef __RENDERER_H
#define __RENDERER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <graphics/camera.hpp>
#include <graphics/shader_program.hpp>
#include <graphics/texture_loader.hpp>

class Renderer
{
private:
    GLFWwindow *window;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<ShaderProgram> shader;
    std::unique_ptr<TextureLoader> texture_loader;

public:
    void handleFramebufferSizeChange(int width, int height);
    Renderer(int window_x, int window_y);
    ~Renderer();
    void tick();
    void updateCamera(ShaderProgram &shader);
    GLFWwindow *getWindow();
    unsigned int windowWidth;
    unsigned int windowHeight;
    Camera &getCamera();
    ShaderProgram &getShader();
    TextureLoader &getTextureLoader() { return *texture_loader.get(); }
};

#endif
