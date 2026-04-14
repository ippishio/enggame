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
#include <algorithm>

#include <graphics/shader_program.hpp>
#include <graphics/renderer.hpp>
#include <core/engine.hpp>
#include <core/enemy.hpp>
#include <core/player.hpp>
#include <core/projectile.hpp>
#include <core/scene.hpp>
#include <core/game_object.hpp>
#include <graphics/mesh.hpp>
#include <graphics/light.hpp>
#include <memory>

int main()
{
    Engine engine = Engine();
    Renderer &renderer = engine.getRenderer();
    InputSystem &input = engine.getInputSystem();
    Camera &camera = renderer.getCamera();
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();
    Light *keyLight = nullptr;
    Light *fillLight = nullptr;
    Light *rimLight = nullptr;
    Player *player = nullptr;
    int projectile_counter = 0;

    renderer.getTextureLoader().loadTexture("../assets/textures/grass.png", "grass");
    renderer.getTextureLoader().loadTexture("../assets/textures/skybox.png", "skybox");
    renderer.setSkyboxTexture("skybox");
    auto plane_model = std::make_shared<Model>(Mesh::makePlane(glm::vec2(-30.0f, -30.0f), glm::vec2(30.0f, 30.0f), "grass"), "");
    auto cube_model = std::make_shared<Model>(Mesh::makeCube(glm::vec3(0.5f), ""), "");
    auto player_model = std::make_shared<Model>("../assets/models/player/player.glb", "../assets/models/player", false);

    auto populate_scene = [&](Scene &target_scene)
    {
        GameObject &plane = target_scene.create<GameObject>(plane_model, "Plane");
        plane.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        plane.setScale(glm::vec3(1.0f));
        plane.setStatic(true);
        plane.setGravityEnabled(false);
        plane.setHitBoxHalfExtents(glm::vec3(30.0f, 0.02f, 30.0f));
        plane.setHitBoxOffset(glm::vec3(0.0f, -0.02f, 0.0f));

        auto make_dynamic_cube = [&](const std::string &name, glm::vec3 position, glm::vec3 velocity = glm::vec3(0.0f))
        {
            GameObject &cube = target_scene.create<GameObject>(cube_model, name);
            cube.setPosition(position);
            cube.setVelocity(velocity);
            cube.setHitBoxHalfExtents(glm::vec3(0.25f));
            return cube;
        };

        for (int level = 0; level < 7; level++)
        {
            make_dynamic_cube("TowerA_" + std::to_string(level), glm::vec3(-2.4f, 0.25f + level * 0.52f, -0.5f));
            make_dynamic_cube("TowerB_" + std::to_string(level), glm::vec3(2.4f, 0.25f + level * 0.52f, 0.5f));
        }

        for (int level = 0; level < 5; level++)
        {
            make_dynamic_cube("CenterStack_" + std::to_string(level), glm::vec3(0.0f, 0.25f + level * 0.52f, -2.0f));
        }

        for (int i = 0; i < 6; i++)
        {
            float height = 4.0f + i * 0.8f;
            float z_offset = (i % 2 == 0) ? -1.4f : 1.4f;
            float x_velocity = (i % 2 == 0) ? 1.2f : -1.2f;
            make_dynamic_cube("Faller_" + std::to_string(i), glm::vec3(-4.0f + i * 1.6f, height, z_offset), glm::vec3(x_velocity, 0.0f, 0.0f));
        }

        make_dynamic_cube("ProjectileLeft", glm::vec3(-6.0f, 1.0f, 0.0f), glm::vec3(7.0f, 2.0f, 0.0f));
        make_dynamic_cube("ProjectileRight", glm::vec3(6.0f, 1.6f, 0.8f), glm::vec3(-7.5f, 1.0f, -0.5f));
        make_dynamic_cube("ProjectileBack", glm::vec3(0.0f, 2.2f, 6.0f), glm::vec3(0.0f, 0.5f, -7.0f));

        player = &target_scene.create<Player>(player_model, "Player");
        player->setPosition(glm::vec3(0.0f, 0.5f, 4.5f));
        player->setScale(glm::vec3(0.2f));
        for (int i = 0; i < 4; i++)
        {
            Enemy &enemy = target_scene.create<Enemy>(player_model, "Enemy_" + std::to_string(i), 100.0f);
            enemy.setPosition(glm::vec3(-3.0f + i * 2.0f, 0.5f, -5.0f - i * 1.4f));
            enemy.setScale(glm::vec3(0.2f));
            enemy.setHitBoxHalfExtents(glm::vec3(0.275f, 0.55f, 0.275f));
        }

        keyLight = &target_scene.create<Light>(cube_model, "KeyLight");
        keyLight->color = glm::vec3(1.0f, 0.85f, 0.65f);
        keyLight->ambientStrength = 0.18f;
        keyLight->setScale(glm::vec3(0.35f));
        keyLight->setStatic(true);
        keyLight->setGravityEnabled(false);
        keyLight->setHitBoxHalfExtents(glm::vec3(0.175f));

        fillLight = &target_scene.create<Light>(cube_model, "FillLight");
        fillLight->color = glm::vec3(0.45f, 0.65f, 1.0f);
        fillLight->ambientStrength = 0.1f;
        fillLight->setScale(glm::vec3(0.25f));
        fillLight->setStatic(true);
        fillLight->setGravityEnabled(false);
        fillLight->setHitBoxHalfExtents(glm::vec3(0.125f));

        rimLight = &target_scene.create<Light>(cube_model, "RimLight");
        rimLight->color = glm::vec3(0.9f, 0.3f, 0.25f);
        rimLight->ambientStrength = 0.08f;
        rimLight->setScale(glm::vec3(0.3f));
        rimLight->setStatic(true);
        rimLight->setGravityEnabled(false);
        rimLight->setHitBoxHalfExtents(glm::vec3(0.15f));
    };

    populate_scene(*scene);

    player->resetView(camera);
    std::cout << "ok 42\n";

    // plane.lookAt(glm::vec3(1.0f, 1.0f, 1.0f));
    //  cube->lookAt(glm::vec3(1.0f, 2.0f, 1.0f));
    std::time_t timestamp = std::time(nullptr);
    int fps = 0;
    float cnt = 0.0f;
    while (!glfwWindowShouldClose(renderer.getWindow()))
    {
        cnt += 0.01f;
        keyLight->setPosition(glm::vec3(std::cos(cnt) * 4.0f, 2.8f, std::sin(cnt) * 4.0f));
        fillLight->setPosition(glm::vec3(std::cos(cnt * 0.6f + 2.0f) * 3.0f, 1.6f, std::sin(cnt * 0.6f + 2.0f) * 3.0f));
        rimLight->setPosition(glm::vec3(-3.0f, 2.2f + std::sin(cnt * 1.5f) * 0.4f, 3.5f));

        if ((std::time(nullptr) - timestamp) >= 1)
        {
            std::cout << "fps: " << fps << std::endl;
            fps = 0;
            timestamp = std::time(nullptr);
        }
        fps++;
        scene->cleanupDestroyed();
        if (input.isKeyTouched(GLFW_KEY_P))
        {
            player->toggleFreeFly(camera);
        }
        if (input.isKeyTouched(GLFW_KEY_U))
        {
            player->toggleThirdPerson();
        }

        player->update(input, camera, engine.deltaTime);
        if (player->canFire(input, engine.deltaTime))
        {
            Projectile &projectile = scene->create<Projectile>(cube_model, "MinigunRound_" + std::to_string(projectile_counter++));
            projectile.setPosition(player->getMuzzlePosition());
            projectile.setVelocity(player->getLookDirection() * 28.0f);
        }

        for (GameObject *object : scene->getAllObjects())
        {
            if (Projectile *projectile = dynamic_cast<Projectile *>(object))
            {
                projectile->updateLifetime(engine.deltaTime);
                if (glm::length(projectile->position) > 80.0f)
                {
                    projectile->destroy();
                }
            }
        }

        engine.tick(*scene);
        if (input.isKeyTouched(GLFW_KEY_K))
        {
            scene = std::make_unique<Scene>();
            populate_scene(*scene);
            cnt = 0.0f;
            projectile_counter = 0;
            player->resetView(camera);
        }
        // std::cout << "rotation: " << camera.rotation.x << " " << camera.rotation.y << " " << camera.rotation.z << std::endl;
        // std::cout << "position: " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
    }
    return 0;
}
