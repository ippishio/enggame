#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <graphics/mesh.hpp>
#include <graphics/shader_program.hpp>
#include <graphics/texture_loader.hpp>

#include <string>
#include <stdexcept>

unsigned int Mesh::buildMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, biTangent));
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_Weights));

    glBindVertexArray(0);
    return VAO;
}

Mesh &Mesh::makePlane(glm::vec2 pos1, glm::vec2 pos2, const std::string &texture_name)
{
    GLfloat texture_count_x = pos2.x - pos1.x;
    GLfloat texture_count_y = pos2.y - pos1.y;
    GLfloat _vertices[] = {
        // Позиции           // Текстурные координаты
        pos2.x, 0.0f, pos2.y, texture_count_x, texture_count_y, // Верхний правый
        pos2.x, 0.0f, pos1.y, texture_count_x, 0.0f,            // Нижний правый
        pos1.x, 0.0f, pos1.y, 0.0f, 0.0f,                       // Нижний левый
        pos1.x, 0.0f, pos2.y, 0.0f, texture_count_y             // Верхний левый
    };
    std::vector<unsigned int> indices = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    std::vector<Vertex> vertices;
    std::vector<Vertex> normals;
    for (int i = 0; i < 4; i++)
    {
        Vertex vertex;
        vertex.position = glm::vec3(_vertices[5 * i], _vertices[5 * i + 1], _vertices[5 * i + 2]);
        vertex.texCoords = glm::vec2(_vertices[5 * i + 3], _vertices[5 * i + 4]);
        vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        vertices.push_back(vertex);
    }
    std::vector<Texture> textures;
    if (texture_name != "")
    {
        Texture texture = TextureLoader::getInstance().getTexture(texture_name);
        textures = {texture};
    }
    Mesh *new_obj = new Mesh(vertices, indices, textures);
    new_obj->buildMesh();
    return *new_obj;
}

Mesh &Mesh::makeCube(glm::vec3 scale, const std::string &texture_name)
{
    GLfloat _vertices[] = {
        -scale.x * 0.5f, -scale.y * 0.5f, -scale.z * 0.5f, 0.0f, 0.0f,
        scale.x * 0.5f, -scale.y * 0.5f, -scale.z * 0.5f, 1.0f, 0.0f,
        scale.x * 0.5f, scale.y * 0.5f, -scale.z * 0.5f, 1.0f, 1.0f,
        -scale.x * 0.5f, scale.y * 0.5f, -scale.z * 0.5f, 0.0f, 1.0f,
        -scale.x * 0.5f, -scale.y * 0.5f, scale.z * 0.5f, 0.0f, 0.0f,
        scale.x * 0.5f, -scale.y * 0.5f, scale.z * 0.5f, 1.0f, 0.0f,
        scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f, 1.0f, 1.0f,
        -scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f, 0.0f, 1.0f,
        -scale.x * 0.5f, -scale.y * 0.5f, -scale.z * 0.5f, 0.0f, 0.0f,
        -scale.x * 0.5f, -scale.y * 0.5f, scale.z * 0.5f, 1.0f, 0.0f,
        -scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f, 1.0f, 1.0f,
        -scale.x * 0.5f, scale.y * 0.5f, -scale.z * 0.5f, 0.0f, 1.0f,
        scale.x * 0.5f, -scale.y * 0.5f, -scale.z * 0.5f, 0.0f, 0.0f,
        scale.x * 0.5f, -scale.y * 0.5f, scale.z * 0.5f, 1.0f, 0.0f,
        scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f, 1.0f, 1.0f,
        scale.x * 0.5f, scale.y * 0.5f, -scale.z * 0.5f, 0.0f, 1.0f,
        -scale.x * 0.5f, -scale.y * 0.5f, -scale.z * 0.5f, 0.0f, 0.0f,
        scale.x * 0.5f, -scale.y * 0.5f, -scale.z * 0.5f, 1.0f, 0.0f,
        scale.x * 0.5f, -scale.y * 0.5f, scale.z * 0.5f, 1.0f, 1.0f,
        -scale.x * 0.5f, -scale.y * 0.5f, scale.z * 0.5f, 0.0f, 1.0f,
        -scale.x * 0.5f, scale.y * 0.5f, -scale.z * 0.5f, 0.0f, 0.0f,
        scale.x * 0.5f, scale.y * 0.5f, -scale.z * 0.5f, 1.0f, 0.0f,
        scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f, 1.0f, 1.0f,
        -scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f, 0.0f, 1.0f};

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20};
    std::vector<Vertex> vertices;
    for (int i = 0; i < 24; i++)
    {
        Vertex vertex;
        vertex.position = glm::vec3(_vertices[5 * i], _vertices[5 * i + 1], _vertices[5 * i + 2]);
        vertex.texCoords = glm::vec2(_vertices[5 * i + 3], _vertices[5 * i + 4]);
        vertices.push_back(vertex);
    }
    std::vector<Texture> textures;
    if (texture_name != "")
    {
        Texture texture = TextureLoader::getInstance().getTexture(texture_name);
        textures = {texture};
    }
    Mesh *new_obj = new Mesh(vertices, indices, textures);
    new_obj->buildMesh();
    return *new_obj;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->buildMesh();
}

void Mesh::Draw(ShaderProgram &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // активируем текстурный блок, до привязки
        // получаем номер текстуры
        std::string name;
        switch (textures[i].type)
        {
        case aiTextureType::aiTextureType_NORMALS:
            name = "texture_normal" + std::to_string(normalNr++);
            break;
        case aiTextureType::aiTextureType_HEIGHT:
            name = "texture_height" + std::to_string(heightNr++);
            break;
        case aiTextureType::aiTextureType_SPECULAR:
            name = "texture_specular" + std::to_string(specularNr++);
            break;
        default:
        case aiTextureType::aiTextureType_DIFFUSE:
            name = "texture_diffuse" + std::to_string(diffuseNr++);
            break;
        }

        shader.setUniform(name, (int)i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Model::Draw(ShaderProgram &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | (flipUV ? aiProcess_FlipUVs : 0));

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw std::runtime_error("Assimp error: " + std::string(import.GetErrorString()));
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // обработать все полигональные сетки в узле(если есть)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // выполнить ту же обработку и для каждого потомка узла
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
            // tangent
            if (mesh->mTangents)
            {
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.tangent = vector;
            }
            if (mesh->mBitangents)
            {
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.biTangent = vector;
            }
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    TextureLoader &texture_loader = TextureLoader::getInstance();
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = texture_loader.loadModelTextures(material, aiTextureType_DIFFUSE, texture_path);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = texture_loader.loadModelTextures(material, aiTextureType_SPECULAR, texture_path);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = texture_loader.loadModelTextures(material, aiTextureType_HEIGHT, texture_path);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = texture_loader.loadModelTextures(material, aiTextureType_AMBIENT, texture_path);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}
