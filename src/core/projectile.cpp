#include <core/projectile.hpp>

Projectile::Projectile(std::shared_ptr<Model> model, const std::string &name)
    : GameObject(std::move(model), name)
{
    setScale(glm::vec3(0.28f));
    setHitBoxHalfExtents(glm::vec3(0.07f));
    setGravityEnabled(false);
}

void Projectile::updateLifetime(float delta_time)
{
    lifetime -= delta_time;
    if (lifetime <= 0.0f)
    {
        destroy();
    }
}

float Projectile::getDamage() const
{
    return damage;
}
