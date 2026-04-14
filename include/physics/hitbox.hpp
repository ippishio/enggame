#ifndef __HITBOX_H
#define __HITBOX_H

#include <glm/glm.hpp>

class HitBox
{
private:
    glm::vec3 half_extents = glm::vec3(0.5f);
    glm::vec3 center_offset = glm::vec3(0.0f);

public:
    HitBox() = default;
    explicit HitBox(glm::vec3 half_extents) : half_extents(half_extents) {}

    void setHalfExtents(glm::vec3 value)
    {
        half_extents = value;
    }

    const glm::vec3 &getHalfExtents() const
    {
        return half_extents;
    }

    void setCenterOffset(glm::vec3 value)
    {
        center_offset = value;
    }

    const glm::vec3 &getCenterOffset() const
    {
        return center_offset;
    }

    glm::vec3 getMin(const glm::vec3 &center) const
    {
        return center + center_offset - half_extents;
    }

    glm::vec3 getMax(const glm::vec3 &center) const
    {
        return center + center_offset + half_extents;
    }

    bool intersects(const glm::vec3 &center, const HitBox &other, const glm::vec3 &other_center) const
    {
        glm::vec3 min_a = getMin(center);
        glm::vec3 max_a = getMax(center);
        glm::vec3 min_b = other.getMin(other_center);
        glm::vec3 max_b = other.getMax(other_center);

        return min_a.x <= max_b.x && max_a.x >= min_b.x &&
               min_a.y <= max_b.y && max_a.y >= min_b.y &&
               min_a.z <= max_b.z && max_a.z >= min_b.z;
    }
};

#endif
