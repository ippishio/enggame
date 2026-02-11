#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <set>

#include <graphics/renderer.hpp>
#include <graphics/camera.hpp>
#include <graphics/mesh.hpp>
#include <graphics/light.hpp>

#include <core/game_object.hpp>
void Renderer::handleFramebufferSizeChange(int width, int height)
{
    windowHeight = height;
    windowWidth = width;
    std::cout << "x: " << width << " y: " << height << std::endl;
    camera->updateWindow(windowWidth, windowHeight);
    glViewport(0, 0, width, height);
}

Renderer::Renderer(int window_x, int window_y)
{
    std::cout << "before init\n";
    glfwInit();
    std::cout << "after init\n";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);
#endif
    window = glfwCreateWindow(window_x, window_y, "Engineer's battleground", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    camera.reset(new Camera(framebufferWidth, framebufferHeight, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    handleFramebufferSizeChange(framebufferWidth, framebufferHeight);
    glEnable(GL_DEPTH_TEST);
    shader.reset(new ShaderProgram("default"));
    texture_loader.reset(&TextureLoader::getInstance());
    shader->loadShader("../assets/shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
    shader->loadShader("../assets/shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);
    shader->linkProgram();
    ShaderProgram *lightShader = new ShaderProgram("light");
    lightShader->loadShader("../assets/shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
    lightShader->loadShader("../assets/shaders/light_shader.glsl", GL_FRAGMENT_SHADER);
    lightShader->linkProgram();
}

Renderer::~Renderer()
{
    glfwTerminate();
}

void Renderer::updateCamera(ShaderProgram &shader)
{
    glm::vec3 front;
    front.x = cos(glm::radians(camera->rotation.x)) * cos(glm::radians(camera->rotation.y));
    front.y = sin(glm::radians(camera->rotation.x));
    front.z = cos(glm::radians(camera->rotation.x)) * sin(glm::radians(camera->rotation.y));
    camera->front = glm::normalize(front);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();
    shader.setUniform("view", view);
    shader.setUniform("projection", projection);
}

void Renderer::tick()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // std::cout << "shaders total count: " << ShaderProgram::getAllPrograms().size() << std::endl;
    Light *light = Light::getAllLights()[0];
    for (auto shader_it : ShaderProgram::getAllPrograms())
    {
        ShaderProgram &shader = *shader_it;
        std::vector<GameObject *> &gameobjects = shader.getAssignedGameObjects();
        // std::cout << shader.getName();
        // std::cout << " total size: " << gameobjects.size() << std::endl;
        for (int i = 0; i < gameobjects.size(); i++)
        {
            GameObject &obj = *gameobjects[i];
            // std::cout << obj.name << " #" << i << " has model? : ";
            // std::cout << obj.hasModel();
            // std::cout << "rendering with: " << shader.getName() << std::endl;
            if (!obj.hasModel())
                continue;
            shader.use();
            Model &model = obj.getModel();
            glm::mat4 model_matrix = obj.getModelMatrix();
            shader.setUniform("model", model_matrix);
            if (shader.getName() != "light")
            {
                shader.setUniform("lightPos", light->position);
                shader.setUniform("ambientStrength", light->ambientStrength);
                shader.setUniform("cameraPos", camera->position);
            }
            shader.setUniform("lightColor", light->color);
            model.Draw(shader);
            glBindVertexArray(0);
        }
        updateCamera(shader);
    }
    glfwSwapBuffers(window);
}

GLFWwindow *Renderer::getWindow()
{
    return window;
}

Camera &Renderer::getCamera()
{
    return *camera;
}

ShaderProgram &Renderer::getShader()
{
    return *shader;
}
