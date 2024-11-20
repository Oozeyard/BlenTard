#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QString>
#include <QHash>

#include "Node.h"

#include <iostream>

struct Vertex 
{
    QVector3D position;
    QVector3D normal;
    QVector2D texCoords;
    QVector3D tangent;
    QVector3D bitangent;
};

struct Texture 
{
    uint id;
    QString type;
    QString path;
};

class Mesh : public Node, protected QOpenGLFunctions 
{
    Q_OBJECT
public:
    explicit Mesh(
        const QString& name,
        const QVector<Vertex>& vertices,
        const QVector<uint>& indices,
        const QVector<Texture>& textures,
        Node* parent = nullptr
    );
    ~Mesh();

    void draw(QOpenGLShaderProgram* program);

    const QVector<Vertex>& vertices() const { return m_vertices; }
    const QVector<uint>& indices() const { return m_indices; }
    const QVector<Texture>& textures() const { return m_textures; }


private:
    QVector<Vertex> m_vertices;
    QVector<uint> m_indices;
    QVector<Texture> m_textures;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    void setupMesh();
};
