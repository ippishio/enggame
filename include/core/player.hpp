#ifndef __PLAYER_H
#define __PLAYER_H

#include <core/game_object.hpp>
#include <graphics/camera.hpp>
#include <core/input_system.hpp>

#include <memory>

class Player : public GameObject
{
private:
    float yaw = -90.0f;
    float pitch = -8.0f;
    bool third_person = false;
    bool free_fly = false;
    float eye_height = 0.4f;
    float third_person_distance = 3.5f;
    float mouse_sensitivity = 0.05f;
    float walk_speed = 4.0f;
    float sprint_speed = 6.5f;
    float jump_speed = 5.5f;
    float fire_cooldown = 0.0f;
    float minigun_interval = 0.06f;

    glm::vec3 getEyePosition() const;
    void updateLook(InputSystem &input);

public:
    Player(std::shared_ptr<Model> model, const std::string &name = "Player");

    void toggleThirdPerson();
    bool isThirdPerson() const;
    void toggleFreeFly(Camera &camera);
    bool isFreeFly() const;
    void resetView(Camera &camera);
    void update(InputSystem &input, Camera &camera, float delta_time);
    glm::vec3 getLookDirection() const;
    glm::vec3 getMuzzlePosition() const;
    bool canFire(InputSystem &input, float delta_time);
};

#endif
