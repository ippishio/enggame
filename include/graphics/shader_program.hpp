#ifndef __SHADER_PROGRAM_H
#define __SHADER_PROGRAM_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
class ShaderProgram
{
private:
    std::vector<unsigned int> loaded_shaders;

public:
    unsigned int id;
    ShaderProgram();
    void loadShader(std::string path, unsigned int shader_type);
    void linkProgram();
    void use();
    void setUniform(const std::string &name, bool value) const;
    void setUniform(const std::string &name, float value) const;
    void setUniform(const std::string &name, int value) const;
    void setUniform(const std::string &name, glm::mat4 & value) const;
    operator unsigned int() const
    {
        return id;
    }
};

#endif