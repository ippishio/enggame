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

#include <graphics/renderer.hpp>
#include <graphics/camera.hpp>
#include <core/game_object.hpp>

void Renderer::handleFramebufferSizeChange(int width, int height)
{
    windowHeight = height;
    windowWidth = width;
    std::cout << "x: " << width << " y: " << height << std::endl;
    camera->updateWindow(windowHeight, windowWidth);
    glViewport(0, 0, width, height);
}

Renderer::Renderer(int window_x, int window_y)
{
    glfwInit();
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
    shader.reset(new ShaderProgram());
    shader->loadShader("../assets/shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
    shader->loadShader("../assets/shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);
    shader->linkProgram();
}

Renderer::~Renderer()
{
    glfwTerminate();
}

void Renderer::updateCamera()
{
    glm::vec3 front;
    front.x = cos(glm::radians(camera->rotation.x)) * cos(glm::radians(camera->rotation.y));
    front.y = sin(glm::radians(camera->rotation.x));
    front.z = cos(glm::radians(camera->rotation.x)) * sin(glm::radians(camera->rotation.y));
    camera->front = glm::normalize(front);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();
    shader->setUniform("view", view);
    shader->setUniform("projection", projection);
}

void Renderer::tick()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();
    for (auto &obj_it : GameObject::getAllObjects())
    {
        if (obj_it.VAO == 0)
            continue;
        glm::mat4 model_matrix = obj_it.getModelMatrix();
        shader->setUniform("model", model_matrix);
        GLuint texture_id = texture_loader.getTextureId(obj_it.texture);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glBindVertexArray(obj_it.VAO);
        glDrawElements(GL_TRIANGLES, obj_it.vertices_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    updateCamera();
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
