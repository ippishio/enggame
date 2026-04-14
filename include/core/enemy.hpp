#ifndef __ENEMY_H
#define __ENEMY_H

#include <core/game_object.hpp>

#include <memory>

class Enemy : public GameObject
{
private:
    float max_health = 100.0f;
    float health = 100.0f;

public:
    Enemy(std::shared_ptr<Model> model, const std::string &name = "Enemy", float max_health = 100.0f);

    void applyDamage(float amount);
    float getHealth() const;
    float getMaxHealth() const;
    float getHealthRatio() const;
};

#endif
