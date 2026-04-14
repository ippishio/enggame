#include <core/enemy.hpp>

#include <algorithm>

Enemy::Enemy(std::shared_ptr<Model> model, const std::string &name, float max_health)
    : GameObject(std::move(model), name), max_health(max_health), health(max_health)
{
}

void Enemy::applyDamage(float amount)
{
    health = std::max(0.0f, health - amount);
    if (health <= 0.0f)
    {
        destroy();
    }
}

float Enemy::getHealth() const
{
    return health;
}

float Enemy::getMaxHealth() const
{
    return max_health;
}

float Enemy::getHealthRatio() const
{
    if (max_health <= 0.0f)
    {
        return 0.0f;
    }
    return health / max_health;
}
