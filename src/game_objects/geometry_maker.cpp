#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

#include <game_objects/geometry_maker.hpp>
#include <core/game_object.hpp>
#include <graphics/texture_loader.hpp>

unsigned int GeometryMaker::buildVAO(GLfloat vertices[], unsigned int indices[], int v_size, int i_size)
{
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_size, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return VAO;
}

GameObject &GeometryMaker::makePlane(glm::vec2 pos1, glm::vec2 pos2,
                                     const std::string &name, const std::string &texture,
                                     float texture_count_x, float texture_count_y)
{
    GameObject new_obj = GameObject(name, texture);
    GLfloat vertices[] = {
        // Позиции           // Текстурные координаты
        pos2.x, pos2.y, 0.0f, texture_count_x, texture_count_y, // Верхний правый
        pos2.x, pos1.y, 0.0f, texture_count_x, 0.0f,            // Нижний правый
        pos1.x, pos1.y, 0.0f, 0.0f, 0.0f,                       // Нижний левый
        pos1.x, pos2.y, 0.0f, 0.0f, texture_count_y             // Верхний левый
    };
    unsigned int indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    unsigned int VAO = buildVAO(vertices, indices, sizeof(vertices), sizeof(indices));
    new_obj.setVAO(VAO);
    new_obj.vertices_count = 6;
    return new_obj;
}
