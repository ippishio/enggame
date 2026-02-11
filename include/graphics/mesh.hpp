#ifndef __MESH_H
#define __MESH_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <assimp/scene.h>

#include <graphics/texture_loader.hpp>

#include <vector>
#include <string>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 biTangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

class ShaderProgram;
class Mesh
{
private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    int vertices_count = 0;
    unsigned int buildMesh();

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(ShaderProgram &shader);
    static Mesh &makePlane(glm::vec2 pos1, glm::vec2 pos2, const std::string &texture_name = "default");
    static Mesh &makeCube(glm::vec3 scale, const std::string &texture_name = "default");
    unsigned int getVAO() { return VAO; }
    unsigned int getVerticesCount() { return vertices_count; }
};

class Model
{
public:
    Model(char *path, const std::string &texture_directory, bool flipUV = false)
    {
        this->flipUV = flipUV;
        this->texture_path = texture_directory;
        loadModel(path);
    }

    Model(Mesh &mesh, const std::string &texture_name = "default")
    {
        mesh.textures.push_back(TextureLoader::getInstance().getTexture(texture_name));
        this->meshes.push_back(mesh);
    }
    void Draw(ShaderProgram &shader);

private:
    bool flipUV = false;
    std::vector<Mesh> meshes;
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::string texture_path;
};

#endif