#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <graphics/shader_program.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

ShaderProgram::ShaderProgram()
{
    id = glCreateProgram();
}

void ShaderProgram::loadShader(std::string path, unsigned int shader_type)
{
    std::ifstream input(path);
    if (input.is_open())
    {
        std::stringstream buffer;
        buffer << input.rdbuf();
        std::string _buffer = buffer.str();
        const char *shaderSource = _buffer.c_str();
        unsigned int shaderID;
        shaderID = glCreateShader(shader_type);
        glShaderSource(shaderID, 1, &shaderSource, NULL);
        glCompileShader(shaderID);
        int success;
        char infoLog[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
            std::stringstream shader_error_info;
            shader_error_info << "Shader '" << path << "' compilation error " << infoLog << std::endl;
            throw std::runtime_error(shader_error_info.str());
        }
        glAttachShader(id, shaderID);
        loaded_shaders.push_back(shaderID);
    }
    else
    {
        throw std::runtime_error("Can't open shader file");
    }
}

void ShaderProgram::linkProgram()
{
    glLinkProgram(id);
    int success;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);

        std::stringstream shader_error_info;
        shader_error_info << "Shader linking " << infoLog << std::endl;
        throw std::runtime_error(shader_error_info.str());
    }
    for (auto &it : loaded_shaders)
    {
        glDeleteShader(it);
    }
}

void ShaderProgram::use()
{
    glUseProgram(id);
}

void ShaderProgram::setUniform(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void ShaderProgram::setUniform(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string &name, glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}