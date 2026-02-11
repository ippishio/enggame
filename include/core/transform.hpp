#ifndef __TRANSFORM_H
#define __TRANSFORM_H

#include <glm/glm.hpp>

class Transform
{
protected:
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _rotation = glm::vec3(0.0f);

public:
    const glm::vec3 &position;
    const glm::vec3 &rotation;
    virtual void setPosition(glm::vec3 position);
    virtual void setRotation(glm::vec3 rotation, bool radians = true);
    void lookAt(glm::vec3 pos);
    Transform() : position(_position),
                  rotation(_rotation) {};
};

#endif