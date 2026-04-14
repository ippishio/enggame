#ifndef __PHYSICS_ENGINE_H
#define __PHYSICS_ENGINE_H

#include <core/scene.hpp>

class PhysicsEngine
{
private:
    void resolveCollision(GameObject &first, GameObject &second);

public:
    void tick(Scene &scene, float delta_time);
};

#endif
