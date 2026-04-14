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
#include <algorithm>

#include <graphics/renderer.hpp>
#include <graphics/camera.hpp>
#include <core/enemy.hpp>
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
    texture_loader = &TextureLoader::getInstance();
    shader->loadShader("../assets/shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
    shader->loadShader("../assets/shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);
    shader->linkProgram();
    light_shader.reset(new ShaderProgram("light"));
    light_shader->loadShader("../assets/shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
    light_shader->loadShader("../assets/shaders/light_shader.glsl", GL_FRAGMENT_SHADER);
    light_shader->linkProgram();
    skybox_shader.reset(new ShaderProgram("skybox"));
    skybox_shader->loadShader("../assets/shaders/skybox_vertex_shader.glsl", GL_VERTEX_SHADER);
    skybox_shader->loadShader("../assets/shaders/skybox_fragment_shader.glsl", GL_FRAGMENT_SHADER);
    skybox_shader->linkProgram();
    health_bar_model = std::make_shared<Model>(Mesh::makeCube(glm::vec3(1.0f, 1.0f, 1.0f), ""), "");
    skybox_mesh = std::make_unique<Mesh>(Mesh::makeCube(glm::vec3(1.0f), ""));
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
    drawSkybox();
    const auto &lights = Light::getAllLights();
    constexpr std::size_t kMaxLights = 8;
    for (auto shader_it : ShaderProgram::getAllPrograms())
    {
        ShaderProgram &shader = *shader_it;
        std::vector<GameObject *> &gameobjects = shader.getAssignedGameObjects();
        shader.use();
        updateCamera(shader);

        if (shader.getName() != "light")
        {
            const int light_count = static_cast<int>(std::min(lights.size(), kMaxLights));
            shader.setUniform("cameraPos", camera->position);
            shader.setUniform("skyLightColor", glm::vec3(0.34f, 0.46f, 0.62f));
            shader.setUniform("groundLightColor", glm::vec3(0.10f, 0.09f, 0.08f));
            shader.setUniform("sunDirection", glm::normalize(glm::vec3(-0.55f, -1.0f, -0.35f)));
            shader.setUniform("sunColor", glm::vec3(1.0f, 0.94f, 0.82f));
            shader.setUniform("sunStrength", 0.5f);
            shader.setUniform("lightCount", light_count);
            for (int i = 0; i < light_count; i++)
            {
                shader.setUniform("lights[" + std::to_string(i) + "].position", lights[i]->position);
                shader.setUniform("lights[" + std::to_string(i) + "].color", lights[i]->color);
                shader.setUniform("lights[" + std::to_string(i) + "].ambientStrength", lights[i]->ambientStrength);
            }
        }

        for (int i = 0; i < gameobjects.size(); i++)
        {
            GameObject &obj = *gameobjects[i];
            if (!obj.hasModel() || !obj.isVisible())
                continue;

            Model &model = obj.getModel();
            glm::mat4 model_matrix = obj.getModelMatrix();
            shader.setUniform("model", model_matrix);

            if (shader.getName() == "light")
            {
                if (Light *light = dynamic_cast<Light *>(&obj))
                {
                    shader.setUniform("lightColor", light->color);
                }
                else
                {
                    shader.setUniform("lightColor", glm::vec3(1.0f));
                }
            }

            model.Draw(shader);
            glBindVertexArray(0);
        }
    }
    drawEnemyHealthBars();
    glfwSwapBuffers(window);
}

void Renderer::drawSkybox()
{
    if (skybox_texture == 0)
    {
        return;
    }

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    skybox_shader->use();
    updateCamera(*skybox_shader);
    skybox_shader->setUniform("skyboxTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skybox_texture);

    glm::mat4 model_matrix = glm::mat4(1.0f);
    skybox_shader->setUniform("model", model_matrix);
    skybox_mesh->Draw(*skybox_shader);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void Renderer::drawEnemyHealthBars()
{
    ShaderProgram &bar_shader = ShaderProgram::getProgramByName("light");
    bar_shader.use();
    updateCamera(bar_shader);
    glm::mat4 billboard_rotation = glm::inverse(glm::mat4(glm::mat3(camera->getViewMatrix())));

    glDisable(GL_DEPTH_TEST);

    for (GameObject *object : GameObject::getAllObjects())
    {
        Enemy *enemy = dynamic_cast<Enemy *>(object);
        if (enemy == nullptr || enemy->isDestroyed() || !enemy->isVisible())
        {
            continue;
        }

        const glm::vec3 bar_base_position = enemy->position + glm::vec3(0.0f, enemy->scale.y * 0.65f + 0.35f, 0.0f);
        const glm::vec3 fill_forward_offset = camera->front * 0.02f;

        glm::mat4 background_model = glm::mat4(1.0f);
        background_model = glm::translate(background_model, bar_base_position);
        background_model *= billboard_rotation;
        background_model = glm::scale(background_model, glm::vec3(0.9f, 0.08f, 0.01f));
        bar_shader.setUniform("model", background_model);
        bar_shader.setUniform("lightColor", glm::vec3(0.15f, 0.05f, 0.05f));
        health_bar_model->Draw(bar_shader);

        float health_ratio = enemy->getHealthRatio();
        glm::vec3 fill_scale = glm::vec3(0.82f * health_ratio, 0.05f, 0.05f);
        glm::vec3 fill_position = bar_base_position + fill_forward_offset + glm::vec3(-(0.82f - fill_scale.x) * 0.5f, 0.0f, 0.0f);
        glm::mat4 fill_model = glm::mat4(1.0f);
        fill_model = glm::translate(fill_model, fill_position);
        fill_model *= billboard_rotation;
        fill_model = glm::scale(fill_model, glm::vec3(fill_scale.x, fill_scale.y, 0.01f));
        glm::vec3 low_health_color = glm::vec3(0.95f, 0.18f, 0.12f);
        glm::vec3 full_health_color = glm::vec3(0.18f, 0.92f, 0.28f);
        glm::vec3 fill_color = glm::mix(low_health_color, full_health_color, health_ratio);
        bar_shader.setUniform("model", fill_model);
        bar_shader.setUniform("lightColor", fill_color);
        health_bar_model->Draw(bar_shader);
    }

    glEnable(GL_DEPTH_TEST);
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

void Renderer::setSkyboxTexture(const std::string &texture_name)
{
    skybox_texture = texture_loader->getTexture(texture_name).id;
    glBindTexture(GL_TEXTURE_2D, skybox_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
