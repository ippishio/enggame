#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/game_object.hpp>
#include <graphics/mesh.hpp>
#include <graphics/shader_program.hpp>

#include <string>
#include <iostream>
#include <algorithm>

std::vector<GameObject *> GameObject::objects;

GameObject::GameObject(const std::string &name, const std::string &shader)
    : scale(_scale)
{
    this->name = name;
    this->texture = "default";
    objects.push_back(this);
    assignToShader(shader);
}

GameObject::GameObject(std::shared_ptr<Model> model, const std::string &name, const std::string &textur, const std::string &shader)
    : scale(_scale)
{
    this->name = name;
    this->texture = textur;
    this->model = std::move(model);
    objects.push_back(this);
    assignToShader(shader);
    updateHitBoxFromModel();
}

GameObject::~GameObject()
{
    ShaderProgram &assigned_shader = ShaderProgram::getProgramByName(shader);
    assigned_shader.removeGameObject(this);
    objects.erase(std::remove(objects.begin(), objects.end(), this), objects.end());
}

void GameObject::destroy()
{
    destroyed = true;
}

GameObject &GameObject::getByName(const std::string &name)
{
    for (const auto &obj : GameObject::objects)
    {
        if (obj->name == name)
        {
            return *obj;
        }
    }
    throw std::runtime_error("GameObject with name '" + name + "' not found");
}

std::vector<GameObject *> GameObject::getAllByName(const std::string &name)
{
    std::vector<GameObject *> ans;
    for (const auto obj : GameObject::objects)
    {
        if (obj->name == name)
        {
            ans.push_back(obj);
        }
    }
    return ans;
}

std::vector<GameObject *> GameObject::getAllObjects()
{

    return objects;
}

void GameObject::setPosition(glm::vec3 position)
{
    Transform::setPosition(position);
}
void GameObject::setRotation(glm::vec3 rotation, bool radians)
{
    Transform::setRotation(rotation, radians);
}
void GameObject::setScale(glm::vec3 scale)
{
    _scale = scale;
    if (!manual_hitbox)
    {
        updateHitBoxFromModel();
    }
}

void GameObject::setVelocity(glm::vec3 new_velocity)
{
    velocity = new_velocity;
}

const glm::vec3 &GameObject::getVelocity() const
{
    return velocity;
}

void GameObject::setStatic(bool value)
{
    static_body = value;
    if (static_body)
    {
        velocity = glm::vec3(0.0f);
    }
}

bool GameObject::isStatic() const
{
    return static_body;
}

void GameObject::setGravityEnabled(bool value)
{
    gravity_enabled = value;
}

bool GameObject::isGravityEnabled() const
{
    return gravity_enabled;
}

bool GameObject::isGrounded() const
{
    return grounded;
}

void GameObject::setGrounded(bool value)
{
    grounded = value;
}

void GameObject::setVisible(bool value)
{
    visible = value;
}

bool GameObject::isVisible() const
{
    return visible;
}

HitBox &GameObject::getHitBox()
{
    return hitbox;
}

const HitBox &GameObject::getHitBox() const
{
    return hitbox;
}

void GameObject::setHitBoxHalfExtents(glm::vec3 half_extents)
{
    manual_hitbox = true;
    hitbox.setHalfExtents(half_extents);
}

void GameObject::setHitBoxOffset(glm::vec3 offset)
{
    manual_hitbox = true;
    hitbox.setCenterOffset(offset);
}

void GameObject::fitHitBoxToModel()
{
    manual_hitbox = false;
    updateHitBoxFromModel();
}

void GameObject::updateHitBoxFromModel()
{
    if (!model)
    {
        return;
    }

    glm::vec3 abs_scale = glm::abs(scale);
    glm::vec3 half_extents = model->getBoundsHalfExtents() * abs_scale;
    glm::vec3 center_offset = model->getBoundsCenter() * scale;

    const float min_extent = 0.01f;
    half_extents.x = std::max(half_extents.x, min_extent);
    half_extents.y = std::max(half_extents.y, min_extent);
    half_extents.z = std::max(half_extents.z, min_extent);

    hitbox.setHalfExtents(half_extents);
    hitbox.setCenterOffset(center_offset);
}

glm::mat4 GameObject::getModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}

void GameObject::assignToShader(const std::string &name)
{
    ShaderProgram &shader = ShaderProgram::getProgramByName(name);
    shader.assignGameObject(this);
    this->shader = name;
}
