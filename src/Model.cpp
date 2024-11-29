#include "Model.h"

Model::Model(const QString& name, const QString& path, Node* parent) :
    Node(name, parent)
{
    initializeOpenGLFunctions();
    loadModel(path);
}

Model::~Model()
{
    for (auto& texture : m_texturesLoaded) {
        glDeleteTextures(1, &texture.id);
    }

    for (auto& child : m_children) {
        delete child;
    }

    m_texturesLoaded.clear();
    m_children.clear();
}


void Model::loadModel(const QString& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toStdString().c_str(),
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = QFileInfo(path).absolutePath();
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (uint i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        addChild(processMesh(mesh, scene));
    }

    for (uint i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}


Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    QVector<Vertex> vertices;
    QVector<uint> indices;
    Material material;

    // Get vertices
    for (uint i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = QVector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = QVector3D(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = QVector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.tangent = QVector3D(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            vertex.bitangent = QVector3D(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        } else {
            vertex.texCoords = QVector2D(0.0f, 0.0f); // Default texture coordinates
        }

        vertices.append(vertex);
    }

    // Get indices
    for (uint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint j = 0; j < face.mNumIndices; j++) {
            indices.append(face.mIndices[j]);
        }
    }

    // Get materials
    if (mesh->mMaterialIndex != UINT_MAX) {
        aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];

        aiColor3D color;
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
            material.albedo = QVector3D(color.r, color.g, color.b);
        } else {
            material.albedo = QVector3D(1.0f, 1.0f, 1.0f); // Default albedo color
        }

        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
            material.specular = QVector3D(color.r, color.g, color.b);
        } else {
            material.specular = QVector3D(0.5f, 0.5f, 0.5f); // Default specular color
        }

        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_SHININESS, material.shininess)) {
            material.shininess = qMax(material.shininess, 32.0f); // Default shininess
        } else {
            material.shininess = 32.0f;
        }

        // Default values
        auto appendTextures = [&](aiTextureType type, const QString& name) {
            QVector<Texture> textures = loadMaterialTextures(aiMaterial, type, name);
            material.textures.append(textures);
        };

        appendTextures(aiTextureType_DIFFUSE, "texture_diffuse");
        appendTextures(aiTextureType_NORMALS, "texture_normal");
        appendTextures(aiTextureType_METALNESS, "texture_metallic");
        appendTextures(aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
    }

    QString meshName = QString(mesh->mName.C_Str());
    return new Mesh(meshName, vertices, indices, material);
}



QVector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const QString& typeName)
{
    QVector<Texture> textures;

    for (uint i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        QString path = QString(str.C_Str());
        bool skip = false;

        for (const auto& loadedTexture : m_texturesLoaded) {
            if (loadedTexture.path == path) {
                textures.append(loadedTexture);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.id = textureFromFile(path, m_directory);
            texture.type = typeName;
            texture.path = path;

            textures.append(texture);
            m_texturesLoaded.append(texture);
        }
    }

    return textures;
}

uint Model::textureFromFile(const QString& path, const QString& directory)
{
    QString filename = QString(path).section('/', -1);
    filename = directory + '/' + filename;

    #ifdef _WIN32
        filename = filename.replace("/", "\\");
    #else
        filename = filename.replace("\\", "/");
    #endif

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.toStdString().c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGB; // Default initialization
        if (nrComponents == 1) {
            format = GL_RED;
        } else if (nrComponents == 3) {
            format = GL_RGB;
        } else if (nrComponents == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Texture failed to load at path: " << path.toStdString() << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}