#include <graphics/light.hpp>

#include <core/game_object.hpp>
#include <graphics/shader_program.hpp>
#include <graphics/mesh.hpp>

#include <string>

std::vector<Light *> Light::instances;

Light::Light(const std::string &name)
    : GameObject(name, "light")
{
    instances.push_back(this);
    light_obj_it = instances.end();
}

Light::Light(Model *model, const std::string &name, const std::string &texture) : GameObject(model, name, texture, "light")
{
    instances.push_back(this);
    light_obj_it = instances.end();
}

void Light::destroy()
{
    instances.erase(light_obj_it);
    GameObject::destroy();
}

std::vector<Light *> Light::getAllLights()
{
    return instances;
}