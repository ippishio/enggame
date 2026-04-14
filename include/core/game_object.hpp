#ifndef __GAME_OBJECT_H
#define __GAME_OBJECT_H

#include <glm/glm.hpp>
#include <graphics/mesh.hpp>
#include <core/transform.hpp>
#include <physics/hitbox.hpp>

#include <vector>
#include <string>
#include <memory>

class GameObject : public Transform
{
protected:
    static std::vector<GameObject *> objects;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 _scale = glm::vec3(1.0f);
    std::shared_ptr<Model> model;
    std::string shader = "default";
    bool destroyed = false;
    glm::vec3 velocity = glm::vec3(0.0f);
    HitBox hitbox;
    bool static_body = false;
    bool gravity_enabled = true;
    bool grounded = false;
    bool visible = true;
    bool manual_hitbox = false;
    void updateHitBoxFromModel();

public:
    std::string name;
    std::string texture;
    GameObject(const std::string &name = "GameObject", const std::string &shader = "default");
    GameObject(std::shared_ptr<Model> model, const std::string &name = "GameObject", const std::string &texture = "default", const std::string &shader = "default");
    virtual ~GameObject();
    void destroy();
    static GameObject &getByName(const std::string &name);
    static std::vector<GameObject *> getAllByName(const std::string &name);
    static std::vector<GameObject *> getAllObjects();
    virtual void setPosition(glm::vec3 position);
    virtual void setRotation(glm::vec3 rotation, bool radians = false);
    void setScale(glm::vec3 scale);
    void setVelocity(glm::vec3 new_velocity);
    const glm::vec3 &getVelocity() const;
    void setStatic(bool value);
    bool isStatic() const;
    void setGravityEnabled(bool value);
    bool isGravityEnabled() const;
    bool isGrounded() const;
    void setGrounded(bool value);
    void setVisible(bool value);
    bool isVisible() const;
    HitBox &getHitBox();
    const HitBox &getHitBox() const;
    void setHitBoxHalfExtents(glm::vec3 half_extents);
    void setHitBoxOffset(glm::vec3 offset);
    void fitHitBoxToModel();
    void assignToShader(const std::string &shader_name);
    const glm::vec3 &scale;
    Model &getModel()
    {
        return *model;
    };
    bool hasModel()
    {
        return static_cast<bool>(model);
    }
    bool isDestroyed() const { return destroyed; }
    glm::mat4 getModelMatrix();
};

#endif
