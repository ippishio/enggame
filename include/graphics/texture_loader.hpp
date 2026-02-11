#ifndef __TEXTURE_LOADER_H
#define __TEXTURE_LOADER_H

#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <assimp/scene.h>

class Texture
{
public:
    GLuint id;
    aiTextureType type;
    Texture() = default;
    Texture(GLuint id, aiTextureType type)
    {
        this->id = id;
        this->type = type;
    }
};

class TextureLoader
{
private:
    GLuint modelTextureFromFile(const char *path);
    std::map<std::string, Texture> loaded_textures;
    std::string default_texture_path = "../assets/textures/default.png";
    TextureLoader();

public:
    static TextureLoader &getInstance();
    Texture loadTexture(const std::string &path, const std::string &name, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE);
    Texture getTexture(const std::string &name);
    bool isTextureLoaded(const std::string &name);
    std::vector<Texture> loadModelTextures(aiMaterial *mat, aiTextureType type,
                                           const std::string &directory);
    void useTexture(const std::string &name);
    void useDefaultTexture();
    TextureLoader(const TextureLoader &) = delete;
};

#endif