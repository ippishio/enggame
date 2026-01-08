#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <SOIL2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <graphics/shader_program.hpp>
#include <graphics/renderer.hpp>


unsigned int build_vao(GLfloat vertices[], unsigned int indices[], int v_size, int i_size)
{
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VAO;
}
int main()
{
    Renderer renderer = Renderer();
    GLfloat vertices[] = {
        // Позиции          // Цвета             // Текстурные координаты
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f,   // Верхний правый
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f,  // Нижний правый
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Нижний левый
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f   // Верхний левый
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    GLfloat texCoords[] = {
        0.0f, 0.0f, // Нижний левый угол
        1.0f, 0.0f, // Нижний правый угол
        0.5f, 1.0f  // Верхняя центральная сторона
    };
    int width, height = 256;
    unsigned char *image = SOIL_load_image("../assets/textures/grid.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    GLuint modelLoc = glGetUniformLocation(renderer.shader->id, "model");
    glm::mat4 model;
    unsigned int VAO1 = build_vao(vertices, indices, sizeof(vertices), sizeof(indices));
    unsigned int VAO2 = build_vao(vertices, indices, sizeof(vertices), sizeof(indices));
    glBindVertexArray(0);
    while (!glfwWindowShouldClose(renderer.window))
    {

        // render
        // ------
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // draw our first triangle
        renderer.shader->use();
        model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5));
        model = glm::rotate(model, (GLfloat)glfwGetTime() * 5.0f, glm::vec3(0.0, 1.0, 1.0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        std::cout << "main loop\n";
        renderer.updateCamera();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        model = glm::scale(glm::mat4(1.0f), glm::vec3(glm::sin((GLfloat)glfwGetTime()) * 2, 0.5, 0.5));
        model = glm::translate(model, glm::vec3(1, 1, 1));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindTexture(GL_TEXTURE_2D, texture);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        renderer.tick();
    }
    return 0;
}