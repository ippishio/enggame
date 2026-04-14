#ifndef __PROJECTILE_H
#define __PROJECTILE_H

#include <core/game_object.hpp>

#include <memory>

class Projectile : public GameObject
{
private:
    float lifetime = 2.5f;
    float damage = 34.0f;

public:
    Projectile(std::shared_ptr<Model> model, const std::string &name = "Projectile");
    void updateLifetime(float delta_time);
    float getDamage() const;
};

#endif
