#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <graphics/shader_program.hpp>
#include <core/game_object.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>

std::map<std::string, ShaderProgram *> ShaderProgram::program_names;

ShaderProgram::ShaderProgram(const std::string &name)
{
    id = glCreateProgram();
    program_names[name] = this;
    this->name = name;
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

void ShaderProgram::setUniform(const std::string &name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

void ShaderProgram::setUniform(const std::string &name, glm::mat4 &value) const
{
    setUniform(id, name, value);
}

void ShaderProgram::setUniform(unsigned int id, const std::string &name, const glm::mat4 &value)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

std::vector<GameObject *>::iterator ShaderProgram::assignGameObject(GameObject *obj)
{
    assigned_gameobjects.push_back(obj);
    return assigned_gameobjects.end();
}
void ShaderProgram::removeGameObject(std::vector<GameObject *>::iterator obj_it)
{
    assigned_gameobjects.erase(obj_it);
}

std::vector<GameObject *> &ShaderProgram::getAssignedGameObjects()
{
    return assigned_gameobjects;
}

ShaderProgram &ShaderProgram::getProgramByName(const std::string &name)
{
    return *program_names[name];
}

std::vector<ShaderProgram *> ShaderProgram::getAllPrograms()
{
    std::vector<ShaderProgram *> shaders;
    for (const auto &pair : program_names)
    {
        shaders.push_back(pair.second);
    }
    return shaders;
}
