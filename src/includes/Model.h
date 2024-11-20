#pragma once

#include <QString>
#include <QVector>
// #include <QSharedPointer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QFileInfo>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "Node.h"
#include "Mesh.h"
#include <iostream>

class Model : public Node, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Model(const QString& name, const QString& path, Node* parent = nullptr);
    ~Model();

    // void draw(QOpenGLShaderProgram* program);

private:
    QVector<Texture> m_texturesLoaded;
    QString m_directory;

    void loadModel(const QString& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    QVector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const QString& typeName);
    uint textureFromFile(const QString& path, const QString& directory);
};