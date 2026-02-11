#ifndef __LIGHT_H
#define __LIGHT_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <core/game_object.hpp>
#include <graphics/mesh.hpp>

class Light : public GameObject
{
private:
    static std::vector<Light *> instances;
    std::vector<Light *>::iterator light_obj_it;

public:
    glm::vec3 color = glm::vec3(1.0f);
    float ambientStrength = 0.1;
    Light(const std::string &name = "Light");
    Light(Model *model, const std::string &name = "Light", const std::string &texture = "");
    static std::vector<Light *> getAllLights();
    void destroy();
};

#endif