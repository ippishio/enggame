#ifndef __LIGHT_H
#define __LIGHT_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <core/game_object.hpp>
#include <graphics/mesh.hpp>
#include <memory>

class Light : public GameObject
{
private:
    static std::vector<Light *> instances;

public:
    glm::vec3 color = glm::vec3(1.0f);
    float ambientStrength = 0.1;
    Light(const std::string &name = "Light");
    Light(std::shared_ptr<Model> model, const std::string &name = "Light", const std::string &texture = "");
    ~Light() override;
    static const std::vector<Light *> &getAllLights();
};

#endif
