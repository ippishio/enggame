#ifndef __GAME_OBJECT_H
#define __GAME_OBJECT_H

#include <glm/glm.hpp>
#include <graphics/mesh.hpp>
#include <core/transform.hpp>

#include <vector>
#include <string>

class GameObject : public Transform
{
protected:
    static std::vector<GameObject *> objects;
    std::vector<GameObject *>::iterator obj_it;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 _scale = glm::vec3(0.0f);
    std::unique_ptr<Model> model;
    std::string shader = "default";
    std::vector<GameObject *>::iterator shader_it;

public:
    std::string name;
    std::string texture;
    GameObject(const std::string &name = "GameObject", const std::string &shader = "default");
    GameObject(Model *model, const std::string &name = "GameObject", const std::string &texture = "default", const std::string &shader = "default");
    void destroy();
    static GameObject &getByName(std::string &name);
    static std::vector<GameObject *> &getAllByName(std::string &name);
    static std::vector<GameObject *> getAllObjects();
    virtual void setPosition(glm::vec3 position);
    virtual void setRotation(glm::vec3 rotation, bool radians = false);
    void setScale(glm::vec3 scale);
    void assignToShader(const std::string &shader_name);
    const glm::vec3 &scale;
    Model &getModel()
    {
        return *this->model.get();
    };
    bool hasModel()
    {
        return (bool)model;
    }
    glm::mat4 getModelMatrix();
};

#endif