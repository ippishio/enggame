#ifndef __SCENE_H
#define __SCENE_H

#include <core/game_object.hpp>
#include <graphics/light.hpp>

#include <memory>
#include <vector>
#include <utility>
#include <cstddef>

class Scene
{
private:
    std::vector<std::unique_ptr<GameObject>> objects;

public:
    Scene() = default;

    template <typename T, typename... Args>
    T &create(Args &&...args)
    {
        static_assert(std::is_base_of_v<GameObject, T>, "Scene can create only GameObject-derived types");
        auto object = std::make_unique<T>(std::forward<Args>(args)...);
        T &reference = *object;
        objects.push_back(std::move(object));
        return reference;
    }

    void cleanupDestroyed();
    std::vector<GameObject *> getAllObjects();
    std::size_t size() const;
};

#endif
