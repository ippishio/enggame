#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <stdexcept>

#include <graphics/renderer.hpp>
#include <graphics/camera.hpp>

void Renderer::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    if (renderer)
    {
        renderer->handleFramebufferSizeChange(width, height);
    }
}

void Renderer::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    if (renderer)
    {
        renderer->handleInput(key, scancode, action, mode);
    }
}

void Renderer::handleFramebufferSizeChange(int width, int height)
{
    windowHeight = height;
    windowWidth = width;
    camera->updateWindow(windowHeight, windowWidth);
    glViewport(0, 0, width, height);
}

void Renderer::handleInput(int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
        pressedKeys[key] = true;
    else if (action == GLFW_RELEASE)
        pressedKeys[key] = false;
}

Renderer::Renderer(int window_x, int window_y)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(window_x, window_y, "Engineer's battleground", NULL, NULL);
    glfwSetWindowUserPointer(window, this);
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
    camera = new Camera(window_x, window_y, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    handleFramebufferSizeChange(window_x, window_y);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    shader = new ShaderProgram();
    shader->loadShader("../assets/shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
    shader->loadShader("../assets/shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);
    shader->linkProgram();
    viewLocId = glGetUniformLocation(shader->id, "view");
    projectionLocId = glGetUniformLocation(shader->id, "projection");
}

Renderer::~Renderer()
{
    glfwTerminate();
}

void Renderer::updateCamera()
{
    float _cameraSpeed = cameraSpeed * deltaTime;
    if (pressedKeys[GLFW_KEY_W])
        camera->position += _cameraSpeed * camera->front;
    if (pressedKeys[GLFW_KEY_S])
        camera->position -= _cameraSpeed * camera->front;
    if (pressedKeys[GLFW_KEY_A])
        camera->position -= glm::normalize(glm::cross(camera->front, camera->up)) * _cameraSpeed;
    if (pressedKeys[GLFW_KEY_D])
        camera->position += glm::normalize(glm::cross(camera->front, camera->up)) * _cameraSpeed;
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();
    glUniformMatrix4fv(viewLocId, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocId, 1, GL_FALSE, glm::value_ptr(projection));
}

void Renderer::tick()
{
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glfwSwapBuffers(window);
}