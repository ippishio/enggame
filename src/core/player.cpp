#include <core/player.hpp>

#include <algorithm>

Player::Player(std::shared_ptr<Model> model, const std::string &name)
    : GameObject(std::move(model), name)
{
    setScale(glm::vec3(1.0f));
    setHitBoxHalfExtents(glm::vec3(0.25f, 0.5f, 0.25f));
    setVisible(false);
}

glm::vec3 Player::getEyePosition() const
{
    return position + glm::vec3(0.0f, eye_height, 0.0f);
}

glm::vec3 Player::getLookDirection() const
{
    float yaw_radians = glm::radians(yaw);
    float pitch_radians = glm::radians(pitch);
    return glm::normalize(glm::vec3(
        std::cos(pitch_radians) * std::cos(yaw_radians),
        std::sin(pitch_radians),
        std::cos(pitch_radians) * std::sin(yaw_radians)));
}

void Player::updateLook(InputSystem &input)
{
    if (!input.isCursorDisabled())
    {
        input.getMouseOffset();
        return;
    }

    glm::vec3 mouse_offset(input.getMouseOffset(), 0.0f);
    mouse_offset *= mouse_sensitivity;
    float swap_x = mouse_offset.x;
    mouse_offset.x = mouse_offset.y;
    mouse_offset.y = swap_x;

    pitch = std::clamp(pitch + mouse_offset.x, -89.0f, 89.0f);
    yaw += mouse_offset.y;
}

void Player::toggleThirdPerson()
{
    if (!free_fly)
    {
        third_person = !third_person;
    }
}

bool Player::isThirdPerson() const
{
    return third_person;
}

void Player::toggleFreeFly(Camera &camera)
{
    free_fly = !free_fly;
    if (free_fly)
    {
        third_person = false;
        setStatic(true);
        setGravityEnabled(false);
        setVelocity(glm::vec3(0.0f));
        setVisible(true);
        camera.setPosition(getEyePosition());
        camera.setRotation(glm::vec3(pitch, yaw, 0.0f));
    }
    else
    {
        setStatic(false);
        setGravityEnabled(true);
        setVelocity(glm::vec3(0.0f));
        setVisible(!third_person ? false : true);
        pitch = camera.rotation.x;
        yaw = camera.rotation.y;
        setRotation(glm::vec3(0.0f, yaw, 0.0f));
    }
}

bool Player::isFreeFly() const
{
    return free_fly;
}

void Player::resetView(Camera &camera)
{
    yaw = -90.0f;
    pitch = -8.0f;
    third_person = false;
    free_fly = false;
    fire_cooldown = 0.0f;
    setStatic(false);
    setGravityEnabled(true);
    setVelocity(glm::vec3(0.0f));
    setVisible(false);
    setRotation(glm::vec3(0.0f, yaw, 0.0f));
    camera.setPosition(getEyePosition());
    camera.setRotation(glm::vec3(pitch, yaw, 0.0f));
}

glm::vec3 Player::getMuzzlePosition() const
{
    return getEyePosition() + getLookDirection() * 0.8f;
}

bool Player::canFire(InputSystem &input, float delta_time)
{
    fire_cooldown = std::max(0.0f, fire_cooldown - delta_time);
    if (!input.isCursorDisabled() || !input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        return false;
    }

    if (fire_cooldown > 0.0f)
    {
        return false;
    }

    fire_cooldown = minigun_interval;
    return true;
}

void Player::update(InputSystem &input, Camera &camera, float delta_time)
{
    if (free_fly)
    {
        if (input.isCursorDisabled())
        {
            cameraFreeFly(camera, input, mouse_sensitivity, 5.0f * delta_time);
        }
        else
        {
            input.getMouseOffset();
        }
        return;
    }

    updateLook(input);
    setRotation(glm::vec3(0.0f, yaw, 0.0f));

    float yaw_radians = glm::radians(yaw);
    glm::vec3 forward = glm::normalize(glm::vec3(std::cos(yaw_radians), 0.0f, std::sin(yaw_radians)));
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 move_direction(0.0f);

    if (input.isKeyPressed(GLFW_KEY_W))
        move_direction += forward;
    if (input.isKeyPressed(GLFW_KEY_S))
        move_direction -= forward;
    if (input.isKeyPressed(GLFW_KEY_D))
        move_direction += right;
    if (input.isKeyPressed(GLFW_KEY_A))
        move_direction -= right;

    glm::vec3 player_velocity = getVelocity();
    if (glm::length(move_direction) > 0.0f)
    {
        move_direction = glm::normalize(move_direction);
        float speed = input.isKeyPressed(GLFW_KEY_LEFT_SHIFT) ? sprint_speed : walk_speed;
        player_velocity.x = move_direction.x * speed;
        player_velocity.z = move_direction.z * speed;
    }
    else
    {
        player_velocity.x = 0.0f;
        player_velocity.z = 0.0f;
    }
    setVelocity(player_velocity);

    if (input.isKeyTouched(GLFW_KEY_SPACE) && isGrounded())
    {
        glm::vec3 jump_velocity = getVelocity();
        jump_velocity.y = jump_speed;
        setVelocity(jump_velocity);
    }

    glm::vec3 eye_position = getEyePosition();
    glm::vec3 look_direction = getLookDirection();

    if (third_person)
    {
        setVisible(true);
        glm::vec3 camera_position = eye_position - look_direction * third_person_distance + glm::vec3(0.0f, 0.6f, 0.0f);
        camera.setPosition(camera_position);
        camera.lookAt(eye_position);
    }
    else
    {
        setVisible(false);
        camera.setPosition(eye_position);
        camera.setRotation(glm::vec3(pitch, yaw, 0.0f));
    }
}
