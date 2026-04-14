#include <glm/glm.hpp>

#include <core/transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/game_object.hpp>
#include <graphics/mesh.hpp>
#include <graphics/shader_program.hpp>

#include <string>
#include <iostream>

void Transform::setPosition(glm::vec3 position)
{
    _position = position;
}
void Transform::setRotation(glm::vec3 rotation, bool radians)
{
    if (radians)
    {
        rotation = glm::degrees(rotation);
    }
    _rotation = rotation;
}

void Transform::lookAt(glm::vec3 pos)
{
    glm::vec3 direction = glm::normalize(pos - position);
    float yaw = glm::degrees(atan2(direction.z, direction.x));
    float distanceXZ = sqrt(direction.x * direction.x + direction.z * direction.z);
    float pitch = glm::degrees(atan2(direction.y, distanceXZ));
    setRotation(glm::vec3(pitch, yaw, 0.0f), false);
}
