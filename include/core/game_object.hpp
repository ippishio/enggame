#ifndef __GAME_OBJECT_H
#define __GAME_OBJECT_H

#include <glm/glm.hpp>

#include <vector>
#include <string>

class GameObject
{
private:
    static std::vector<GameObject *> objects;
    std::vector<GameObject *>::iterator obj_it;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _rotation = glm::vec3(0.0f);
    glm::vec3 _scale = glm::vec3(0.0f);

public:
    std::string name;
    std::string texture;

    int vertices_count = 0; // move to class Mesh
    GameObject(const std::string &name = "GameObject", const std::string &texture = "default");
    ~GameObject();
    void destroy();
    static GameObject &getByName(std::string &name);
    static std::vector<GameObject *> &getAllByName(std::string &name);
    static std::vector<GameObject *> &getAllObjects();
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);
    const glm::vec3 &position;
    const glm::vec3 &rotation;
    const glm::vec3 &scale;
    unsigned int VAO = 0;
    void lookAt(glm::vec3 pos);
    glm::mat4 getModelMatrix();
};

#endif