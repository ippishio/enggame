#include <graphics/light.hpp>

#include <core/game_object.hpp>
#include <graphics/shader_program.hpp>
#include <graphics/mesh.hpp>

#include <string>
#include <algorithm>

std::vector<Light *> Light::instances;

Light::Light(const std::string &name)
    : GameObject(name, "light")
{
    instances.push_back(this);
}

Light::Light(std::shared_ptr<Model> model, const std::string &name, const std::string &texture) : GameObject(std::move(model), name, texture, "light")
{
    instances.push_back(this);
}

Light::~Light()
{
    instances.erase(std::remove(instances.begin(), instances.end(), this), instances.end());
}

const std::vector<Light *> &Light::getAllLights()
{
    return instances;
}
