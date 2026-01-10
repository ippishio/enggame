#include <graphics/texture_loader.hpp>

#include <GLFW/glfw3.h>
#include <SOIL2.h>
#include <string.h>
#include <stdexcept>
#include <sstream>

TextureLoader::TextureLoader()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    loadTexture(default_texture_path, "default");
}

GLuint TextureLoader::loadTexture(const std::string &path, const std::string &name)
{
    const char *_path = path.c_str();
    GLuint new_id = SOIL_CREATE_NEW_ID;
    if (loaded_textures.count(name) > 0)
    {
        new_id = loaded_textures[name];
    }
    new_id = SOIL_load_OGL_texture(
        _path,
        SOIL_LOAD_AUTO,
        new_id,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS);
    if (new_id == 0)
    {
        std::stringstream error_info;
        error_info << "Can't load texture '" << path << "', info: " << SOIL_last_result() << std::endl;
        throw std::runtime_error(error_info.str());
    }
    loaded_textures[name] = new_id;
    return new_id;
}

GLuint TextureLoader::getTextureId(const std::string &name)
{
    if (loaded_textures.count(name) > 0)
    {
        return loaded_textures[name];
    }

    return loaded_textures["default"];
}

void TextureLoader::useTexture(const std::string &name)
{
    GLuint texture = getTextureId(name);
    glBindTexture(GL_TEXTURE_2D, texture);
}