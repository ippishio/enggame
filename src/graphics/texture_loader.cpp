#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/texture_loader.hpp>

#include <stb_image/stb_image.h>
#include <assimp/scene.h>
#include <string.h>
#include <stdexcept>
#include <iostream>

TextureLoader &TextureLoader::getInstance()
{
    static TextureLoader instance; // Создается при первом вызове
    return instance;
}

TextureLoader::TextureLoader()
{
    loadTexture(default_texture_path, "default");
}

Texture TextureLoader::loadTexture(const std::string &path, const std::string &name, aiTextureType type)
{
    std::cout << "loading texture path: " << path << ", name: " << name;
    if (isTextureLoaded(name))
    {
        std::cout << std::endl;
        return loaded_textures[name];
    }
    std::cout << ", new one!" << std::endl;
    GLuint textureID = modelTextureFromFile(path.c_str());
    Texture texture(textureID, type);
    loaded_textures[name] = texture;
    return texture;
}

Texture TextureLoader::getTexture(const std::string &name)
{
    if (isTextureLoaded(name))
    {
        return loaded_textures[name];
    }

    return loaded_textures["default"];
}

void TextureLoader::useTexture(const std::string &name)
{
    GLuint texture = getTexture(name).id;
    glBindTexture(GL_TEXTURE_2D, texture);
}

bool TextureLoader::isTextureLoaded(const std::string &name)
{
    return loaded_textures.count(name) > 0;
}

std::vector<Texture> TextureLoader::loadModelTextures(aiMaterial *mat, aiTextureType type,
                                                      const std::string &directory)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString name;
        mat->GetTexture(type, i, &name);
        std::string path = directory + "/" + name.C_Str();
        std::string texName = path;
        try
        {
            Texture texture = loadTexture(path, texName, type);
            textures.push_back(texture);
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "Failed to load texture: " << path << " - " << e.what() << std::endl;
            textures.push_back(getTexture("default"));
        }
    }
    return textures;
}

GLuint TextureLoader::modelTextureFromFile(const char *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 1;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        throw std::runtime_error("Texture failed to load at path: " + std::string(path));
        stbi_image_free(data);
    }

    return textureID;
}