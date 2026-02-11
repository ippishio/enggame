#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/game_object.hpp>
#include <graphics/mesh.hpp>
#include <graphics/shader_program.hpp>

#include <string>
#include <iostream>

std::vector<GameObject *> GameObject::objects;

GameObject::GameObject(const std::string &name, const std::string &shader)
    : scale(_scale)
{
    this->model.reset(nullptr);
    this->name = name;
    this->texture = "default";
    objects.push_back(this);
    obj_it = objects.end();
    assignToShader(shader);
}

GameObject::GameObject(Model *model, const std::string &name, const std::string &textur, const std::string &shader)
    : scale(_scale)
{
    this->name = name;
    this->texture = texture;
    this->model.reset(model);
    objects.push_back(this);
    obj_it = objects.end();
    assignToShader(shader);
}
void GameObject::destroy()
{
    ShaderProgram &shader = ShaderProgram::getProgramByName(this->shader);
    shader.removeGameObject(shader_it);
    objects.erase(obj_it);
    delete this;
}

GameObject &GameObject::getByName(std::string &name)
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

std::vector<GameObject *> &GameObject::getAllByName(std::string &name)
{
    std::vector<GameObject *> *ans = new std::vector<GameObject *>();
    for (const auto obj : GameObject::objects)
    {
        if (obj->name == name)
        {
            ans->push_back(obj);
        }
    }
    return *ans;
}

std::vector<GameObject *> GameObject::getAllObjects()
{

    return objects;
}

void GameObject::setPosition(glm::vec3 position)
{
    Transform::setPosition(position);
    modelMatrix = glm::translate(modelMatrix, position);
}
void GameObject::setRotation(glm::vec3 rotation, bool radians)
{
    Transform::setRotation(rotation, radians);
    modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}
void GameObject::setScale(glm::vec3 scale)
{
    _scale = scale;
    modelMatrix = glm::scale(modelMatrix, scale);
}

glm::mat4 GameObject::getModelMatrix()
{
    return modelMatrix;
}

void GameObject::assignToShader(const std::string &name)
{
    ShaderProgram &shader = ShaderProgram::getProgramByName(name);
    this->shader_it = shader.assignGameObject(this);
    this->shader = name;
}