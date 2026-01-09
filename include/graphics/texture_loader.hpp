#ifndef __TEXTURE_LOADER_H
#define __TEXTURE_LOADER_H

#include <GLFW/glfw3.h>
#include <map>
#include <string>

class TextureLoader
{
private:
    std::map<std::string, GLuint> loaded_textures;
    std::string default_texture_path = "../assets/textures/default.jpg";

public:
    GLuint loadTexture(const std::string &path, const std::string &name);
    GLuint getTextureId(const std::string &name);
    TextureLoader();
    void useTexture(const std::string &name);
    void useDefaultTexture();
};

#endif