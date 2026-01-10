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

GameObject *GeometryMaker::makePlane(glm::vec2 pos1, glm::vec2 pos2,
                                     const std::string &name, const std::string &texture,
                                     float texture_count_x, float texture_count_y)
{
    GameObject *new_obj = new GameObject(name, texture);
    GLfloat vertices[] = {
        // Позиции           // Текстурные координаты
        pos2.x, 0.0f, pos2.y, texture_count_x, texture_count_y, // Верхний правый
        pos2.x, 0.0f, pos1.y, texture_count_x, 0.0f,            // Нижний правый
        pos1.x, 0.0f, pos1.y, 0.0f, 0.0f,                       // Нижний левый
        pos1.x, 0.0f, pos2.y, 0.0f, texture_count_y             // Верхний левый
    };
    unsigned int indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    unsigned int VAO = buildVAO(vertices, indices, sizeof(vertices), sizeof(indices));
    new_obj->VAO = VAO;
    new_obj->vertices_count = 6;
    return new_obj;
}

GameObject *GeometryMaker::makeCube(glm::vec2 pos1, glm::vec2 pos2,
                                    const std::string &name, const std::string &texture,
                                    float texture_count_x, float texture_count_y)
{
    GameObject *new_obj = new GameObject(name, texture);

    float vertices[] = {
        // Позиции (x, y, z)      // Текстурные координаты (u, v)
        // Задняя грань
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // нижний левый
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // нижний правый
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // верхний правый
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // верхний левый

        // Передняя грань
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

        // Левая грань
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

        // Правая грань
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

        // Нижняя грань
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,

        // Верхняя грань
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f};

    // Индексы для 24 вершин
    unsigned int indices[] = {
        // Задняя грань (вершины 0-3)
        0, 1, 2, 2, 3, 0,
        // Передняя грань (вершины 4-7)
        4, 5, 6, 6, 7, 4,
        // Левая грань (вершины 8-11)
        8, 9, 10, 10, 11, 8,
        // Правая грань (вершины 12-15)
        12, 13, 14, 14, 15, 12,
        // Нижняя грань (вершины 16-19)
        16, 17, 18, 18, 19, 16,
        // Верхняя грань (вершины 20-23)
        20, 21, 22, 22, 23, 20};

    unsigned int VAO = buildVAO(vertices, indices, sizeof(vertices), sizeof(indices));
    new_obj->VAO = VAO;
    new_obj->vertices_count = sizeof(indices);
    return new_obj;
}