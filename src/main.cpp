#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <SOIL2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <graphics/shader_program.hpp>
#include <graphics/renderer.hpp>
#include <core/engine.hpp>
#include <core/game_object.hpp>
#include <game_objects/geometry_maker.hpp>

int main()
{
    Engine engine = Engine();
    Renderer &renderer = engine.getRenderer();
    InputSystem &input = engine.getInputSystem();
    Camera &camera = renderer.getCamera();
    renderer.getTextureLoader().loadTexture("../assets/textures/grass.png", "grass");
    GameObject *plane = GeometryMaker::makePlane(glm::vec2(-5.0f, -5.0f),
                                                 glm::vec2(5.0f, 5.0f), "Plane", "grass", 10.0f, 10.0f);
    GameObject *cube = GeometryMaker::makeCube(glm::vec2(-4.0f, -1.0f), glm::vec2(1.0f, 1.0f));
    cube->setPosition(cube->position + glm::vec3(0.0f, 2.0f, 0.0f));
    camera.setPosition(glm::vec3(0.0f, 3.0f, 2.0f));
    camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    while (!glfwWindowShouldClose(renderer.getWindow()))
    {
        GLfloat cameraSpeed = 5.0f * engine.deltaTime;
        cameraFreeFly(camera, input, 0.05f, cameraSpeed);
        engine.tick();
        std::cout << "rotation: " << camera.rotation.x << " " << camera.rotation.y << " " << camera.rotation.z << std::endl;
        std::cout << "position: " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
    }
    return 0;
}