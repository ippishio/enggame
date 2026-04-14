#ifndef __RENDERER_H
#define __RENDERER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <graphics/camera.hpp>
#include <graphics/shader_program.hpp>
#include <graphics/texture_loader.hpp>
#include <memory>

class Renderer
{
private:
    GLFWwindow *window;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<ShaderProgram> shader;
    std::unique_ptr<ShaderProgram> light_shader;
    std::unique_ptr<ShaderProgram> skybox_shader;
    TextureLoader *texture_loader = nullptr;
    std::shared_ptr<Model> health_bar_model;
    std::unique_ptr<Mesh> skybox_mesh;
    unsigned int skybox_texture = 0;
    void drawEnemyHealthBars();
    void drawSkybox();

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
    void setSkyboxTexture(const std::string &texture_name);
    TextureLoader &getTextureLoader() { return *texture_loader; }
};

#endif
