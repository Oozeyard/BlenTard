#pragma once

#include <string>
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "stb_image.h"

class Model {
public:
    Model(const std::string &path);
    ~Model();
    void Draw(GLuint shaderProgram);

private:
    std::vector<Mesh*> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_textures_loaded;

    void loadModel(const std::string &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory);
};