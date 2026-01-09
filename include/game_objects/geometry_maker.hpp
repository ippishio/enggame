#ifndef __GEOMETRY_MAKER_H
#define __GEOMETRY_MAKER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <core/game_object.hpp>

class GeometryMaker
{
public:
    static GameObject &makePlane(glm::vec2 pos1, glm::vec2 pos2,
                                 const std::string &name = "Plane",
                                 const std::string &texture = "default",
                                 float texture_count_x = 1.0f, float texture_count_y = 1.0f);
    static unsigned int buildVAO(GLfloat vertices[], unsigned int indices[], int v_size, int i_size);
};

#endif