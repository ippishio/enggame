#include <physics/physics_engine.hpp>

#include <algorithm>
#include <core/enemy.hpp>
#include <core/projectile.hpp>

namespace
{
constexpr float kGravity = 9.81f;
constexpr float kTerminalVelocity = 50.0f;
constexpr float kGroundFriction = 10.0f;
constexpr float kVelocityStopThreshold = 0.05f;
constexpr int kSolverIterations = 8;
}

void PhysicsEngine::resolveCollision(GameObject &first, GameObject &second)
{
    if (!first.getHitBox().intersects(first.position, second.getHitBox(), second.position))
    {
        return;
    }

    Projectile *first_projectile = dynamic_cast<Projectile *>(&first);
    Projectile *second_projectile = dynamic_cast<Projectile *>(&second);
    Enemy *first_enemy = dynamic_cast<Enemy *>(&first);
    Enemy *second_enemy = dynamic_cast<Enemy *>(&second);

    if (first_projectile != nullptr && second_enemy != nullptr)
    {
        second_enemy->applyDamage(first_projectile->getDamage());
        first_projectile->destroy();
        return;
    }
    if (second_projectile != nullptr && first_enemy != nullptr)
    {
        first_enemy->applyDamage(second_projectile->getDamage());
        second_projectile->destroy();
        return;
    }

    glm::vec3 first_min = first.getHitBox().getMin(first.position);
    glm::vec3 first_max = first.getHitBox().getMax(first.position);
    glm::vec3 second_min = second.getHitBox().getMin(second.position);
    glm::vec3 second_max = second.getHitBox().getMax(second.position);

    float overlap_x = std::min(first_max.x, second_max.x) - std::max(first_min.x, second_min.x);
    float overlap_y = std::min(first_max.y, second_max.y) - std::max(first_min.y, second_min.y);
    float overlap_z = std::min(first_max.z, second_max.z) - std::max(first_min.z, second_min.z);

    if (overlap_x <= 0.0f || overlap_y <= 0.0f || overlap_z <= 0.0f)
    {
        return;
    }

    glm::vec3 correction(0.0f);
    if (overlap_x <= overlap_y && overlap_x <= overlap_z)
    {
        correction.x = (first.position.x < second.position.x) ? -overlap_x : overlap_x;
    }
    else if (overlap_y <= overlap_x && overlap_y <= overlap_z)
    {
        correction.y = (first.position.y < second.position.y) ? -overlap_y : overlap_y;
    }
    else
    {
        correction.z = (first.position.z < second.position.z) ? -overlap_z : overlap_z;
    }

    const bool first_static = first.isStatic();
    const bool second_static = second.isStatic();

    if (first_static && second_static)
    {
        return;
    }

    if (first_static || second_static)
    {
        GameObject &dynamic_object = first_static ? second : first;
        glm::vec3 applied_correction = first_static ? -correction : correction;
        dynamic_object.setPosition(dynamic_object.position + applied_correction);

        glm::vec3 velocity = dynamic_object.getVelocity();
        if (applied_correction.x != 0.0f)
        {
            velocity.x = 0.0f;
        }
        if (applied_correction.y != 0.0f)
        {
            if (applied_correction.y > 0.0f)
            {
                dynamic_object.setGrounded(true);
            }
            velocity.y = 0.0f;
        }
        if (applied_correction.z != 0.0f)
        {
            velocity.z = 0.0f;
        }
        if (dynamic_cast<Projectile *>(&dynamic_object) != nullptr)
        {
            dynamic_object.destroy();
            return;
        }

        dynamic_object.setVelocity(velocity);
        return;
    }

    glm::vec3 half_correction = correction * 0.5f;
    first.setPosition(first.position + half_correction);
    second.setPosition(second.position - half_correction);

    glm::vec3 first_velocity = first.getVelocity();
    glm::vec3 second_velocity = second.getVelocity();

    if (half_correction.x != 0.0f)
    {
        first_velocity.x = 0.0f;
        second_velocity.x = 0.0f;
    }
    if (half_correction.y != 0.0f)
    {
        if (half_correction.y > 0.0f)
        {
            first.setGrounded(true);
        }
        else
        {
            second.setGrounded(true);
        }
        first_velocity.y = 0.0f;
        second_velocity.y = 0.0f;
    }
    if (half_correction.z != 0.0f)
    {
        first_velocity.z = 0.0f;
        second_velocity.z = 0.0f;
    }

    if (first_projectile != nullptr)
    {
        first.destroy();
    }
    else
    {
        first.setVelocity(first_velocity);
    }

    if (second_projectile != nullptr)
    {
        second.destroy();
    }
    else
    {
        second.setVelocity(second_velocity);
    }
}

void PhysicsEngine::tick(Scene &scene, float delta_time)
{
    delta_time = std::clamp(delta_time, 0.0f, 1.0f / 60.0f);
    std::vector<GameObject *> objects = scene.getAllObjects();

    for (GameObject *object : objects)
    {
        if (object->isDestroyed())
        {
            continue;
        }

        object->setGrounded(false);

        if (object->isStatic())
        {
            continue;
        }

        glm::vec3 velocity = object->getVelocity();
        if (object->isGravityEnabled())
        {
            velocity.y = std::max(velocity.y - kGravity * delta_time, -kTerminalVelocity);
        }

        object->setVelocity(velocity);
        object->setPosition(object->position + velocity * delta_time);
    }

    for (int iteration = 0; iteration < kSolverIterations; iteration++)
    {
        for (std::size_t i = 0; i < objects.size(); i++)
        {
            GameObject *first = objects[i];
            if (first->isDestroyed())
            {
                continue;
            }

            for (std::size_t j = i + 1; j < objects.size(); j++)
            {
                GameObject *second = objects[j];
                if (second->isDestroyed())
                {
                    continue;
                }

                resolveCollision(*first, *second);
            }
        }
    }

    for (GameObject *object : objects)
    {
        if (object->isDestroyed() || object->isStatic() || !object->isGrounded())
        {
            continue;
        }

        glm::vec3 velocity = object->getVelocity();
        float damping = std::max(0.0f, 1.0f - kGroundFriction * delta_time);
        velocity.x *= damping;
        velocity.z *= damping;

        if (std::abs(velocity.x) < kVelocityStopThreshold)
        {
            velocity.x = 0.0f;
        }
        if (std::abs(velocity.z) < kVelocityStopThreshold)
        {
            velocity.z = 0.0f;
        }

        object->setVelocity(velocity);
    }
}
