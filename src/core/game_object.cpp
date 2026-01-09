#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <include/core/game_object.hpp>

#include <string>

GameObject::GameObject(const std::string &name, const std::string &texture)
    : position(_position),
      rotation(_rotation),
      scale(_scale)
{
    this->name = name;
    this->texture = texture;
    objects.push_back(this);
    obj_it = objects.end();
}

void GameObject::destroy()
{
    objects.erase(obj_it);
    delete this;
}
GameObject::~GameObject()
{
    destroy();
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

std::vector<GameObject &> &GameObject::getAllByName(std::string &name)
{
    std::vector<GameObject &> ans;
    for (const auto &obj : GameObject::objects)
    {
        if (obj->name == name)
        {
            ans.push_back(*obj);
        }
    }
    return ans;
}

std::vector<GameObject &> &GameObject::getAllObjects()
{
    std::vector<GameObject &> ans;
    for (const auto &obj : GameObject::objects)
    {
        ans.push_back(*obj);
    }
    return ans;
}

void GameObject::setPosition(glm::vec3 position)
{
    _position = position;
    modelMatrix = glm::translate(modelMatrix, position);
}
void GameObject::setRotation(glm::vec3 rotation)
{
    _rotation = rotation;
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