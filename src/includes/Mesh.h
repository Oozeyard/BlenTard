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

struct Material 
{
    QVector3D albedo;
    QVector3D specular;
    QVector3D emissive;
    float shininess;
    float metalness;
    float roughness;

    QVector<Texture> textures;
};

class Mesh : public Node, protected QOpenGLFunctions 
{
    Q_OBJECT
public:
    explicit Mesh(
        const QString& name,
        const QVector<Vertex>& vertices,
        const QVector<uint>& indices,
        const Material& material,
        Node* parent = nullptr
    );
    ~Mesh();

    void draw(QOpenGLShaderProgram* program);

    const QVector<Vertex>& vertices() const { return m_vertices; }
    const QVector<uint>& indices() const { return m_indices; }
    const Material& material() const { return m_material; }

    void subdivide();


private:
    QVector<Vertex> m_vertices;
    QVector<uint> m_indices;
    Material m_material;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    void setupMesh();
};

