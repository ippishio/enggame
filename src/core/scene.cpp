#include <core/scene.hpp>

#include <algorithm>

void Scene::cleanupDestroyed()
{
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
                       [](const std::unique_ptr<GameObject> &object)
                       { return object->isDestroyed(); }),
        objects.end());
}

std::vector<GameObject *> Scene::getAllObjects()
{
    std::vector<GameObject *> result;
    result.reserve(objects.size());
    for (const auto &object : objects)
    {
        result.push_back(object.get());
    }
    return result;
}

std::size_t Scene::size() const
{
    return objects.size();
}
