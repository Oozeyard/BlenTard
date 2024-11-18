#include "Model.h"

Model::Model(const std::string &path) {
    m_meshes.clear();
    loadModel(path);
}

Model::~Model() {
    for (unsigned int i = 0; i < m_meshes.size(); i++)
        delete m_meshes[i];
}

void Model::Draw(GLuint shaderProgram) {
    std::cout << "Drawing model" << std::endl;
    std::cout << "Number of meshes: " << m_meshes.size() << std::endl;
    for (unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes[i]->Draw(shaderProgram);
}

void Model::loadModel(const std::string &path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode || !scene->mMeshes) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    m_directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        unsigned int meshIndex = node->mMeshes[i];
        if (meshIndex < scene->mNumMeshes) {
            aiMesh* mesh = scene->mMeshes[meshIndex];
            std::cout << "Processing mesh " << meshIndex << std::endl;  // Debug log
            m_meshes.push_back(processMesh(mesh, scene));
        }
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = QVector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = QVector3D(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = QVector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.tangent = QVector3D(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            vertex.bitangent = QVector3D(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        } else
            vertex.texCoords = QVector2D(0.0f, 0.0f);
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    std::vector<Texture> metallicMaps = loadMaterialTextures(material, aiTextureType_METALNESS, "texture_metallic");
    textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());
    std::vector<Texture> roughnessMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
    textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

    std::cout<< "Number of vertices: " << vertices.size() << std::endl;
    return new Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < m_textures_loaded.size(); j++) {
            if (std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(m_textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), m_directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    f->glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;

        f->glBindTexture(GL_TEXTURE_2D, textureID);
        f->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        f->glGenerateMipmap(GL_TEXTURE_2D);

        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

