#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <random>
#include <ctime>

#include <graphics/shader_program.hpp>
#include <graphics/renderer.hpp>
#include <core/engine.hpp>
#include <core/game_object.hpp>
#include <graphics/mesh.hpp>
#include <graphics/light.hpp>

int main()
{
    Engine engine = Engine();
    Renderer &renderer = engine.getRenderer();
    InputSystem &input = engine.getInputSystem();
    Camera &camera = renderer.getCamera();
    renderer.getTextureLoader().loadTexture("../assets/textures/grass.png", "grass");
    Model plane_model = Model(Mesh::makePlane(glm::vec2(-5.0f, -5.0f), glm::vec2(5.0f, 5.0f), "grass"));
    Model cube_model = Model(Mesh::makeCube(glm::vec3(0.5f), ""));
    GameObject plane = GameObject(&plane_model, "Plane");
    Model backpack_model("../assets/models/backpack/backpack.obj", "../assets/models/backpack", false);
    GameObject *backpack;
    // for (int k = 0; k < 10; k++)
    //     for (int j = 0; j < 10; j++)
    //         for (int i = 0; i < 10; i++)
    //         {
    //             backpack = new GameObject(&cube_model, "backpack" + std::to_string(i));
    //             std::cout << "ok 34\n";
    //             backpack->setPosition(glm::vec3(1.0f * i, 1.0f * j, 1.0f * k));
    //             // backpack->setScale(glm::vec3(0.3f, 0.3f, 0.3f));
    //         }
    backpack = new GameObject(&backpack_model, "backpack");

    // plane.setPosition(plane.position + glm::vec3(0.0f, 10.0f, 0.0f));
    Light light(&cube_model);
    // std::cout << light.hasShader() << " " << light.hasModel() << std::endl;
    light.color = glm::vec3(1.0f, 1.0f, 1.0f);
    light.ambientStrength = 10.0f;
    light.setPosition(glm::vec3(2.5f));
    camera.setPosition(glm::vec3(5.0f, 5.0f, 5.0f));
    camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    std::cout << "ok 42\n";

    // plane.lookAt(glm::vec3(1.0f, 1.0f, 1.0f));
    //  cube->lookAt(glm::vec3(1.0f, 2.0f, 1.0f));
    std::time_t timestamp = std::time(nullptr);
    int fps = 0;
    float cnt = 0.0f;
    while (!glfwWindowShouldClose(renderer.getWindow()))
    {
        cnt += 0.01f;
        if ((std::time(nullptr) - timestamp) >= 1)
        {
            std::cout << "fps: " << fps << std::endl;
            fps = 0;
            timestamp = std::time(nullptr);
        }
        fps++;
        GLfloat cameraSpeed = 5.0f * engine.deltaTime;
        // light.setPosition(glm::vec3(glm::cos(cnt) * 5.0f, 3.0f, glm::sin(cnt) * 5.0f));

        cameraFreeFly(camera, input, 0.05f, cameraSpeed);

        engine.tick();
        // std::cout << "rotation: " << camera.rotation.x << " " << camera.rotation.y << " " << camera.rotation.z << std::endl;
        // std::cout << "position: " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
    }
    return 0;
}