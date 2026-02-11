#ifndef __SHADER_PROGRAM_H
#define __SHADER_PROGRAM_H

#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <core/game_object.hpp>

class ShaderProgram
{
private:
    std::vector<unsigned int> loaded_shaders;
    static std::map<std::string, ShaderProgram *> program_names;
    std::string name;
    std::vector<GameObject *> assigned_gameobjects;

public:
    unsigned int id;
    ShaderProgram(const std::string &name);
    void loadShader(std::string path, unsigned int shader_type);
    void linkProgram();
    void use();
    void setUniform(const std::string &name, bool value) const;
    void setUniform(const std::string &name, float value) const;
    void setUniform(const std::string &name, int value) const;
    void setUniform(const std::string &name, glm::mat4 &value) const;
    void setUniform(const std::string &name, glm::vec3 value) const;
    std::vector<GameObject *>::iterator assignGameObject(GameObject *obj);
    void removeGameObject(std::vector<GameObject *>::iterator obj_it);
    std::vector<GameObject *> &getAssignedGameObjects();
    static void setUniform(unsigned int id, const std::string &name, const glm::mat4 &value);
    static ShaderProgram &getProgramByName(const std::string &name);
    static std::vector<ShaderProgram *> getAllPrograms();
    std::string &getName()
    {
        return name;
    }
    operator unsigned int() const
    {
        return id;
    }
};

#endif