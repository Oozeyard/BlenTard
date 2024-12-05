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
#include <QSet>
#include <cfloat>

#include "Node.h"
#include "Utils.h"

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

    void setAlbedo(const QVector3D& color) { albedo = color; }
    void setSpecular(const QVector3D& color) { specular = color; }
    void setEmissive(const QVector3D& color) { emissive = color; }
    void setShininess(float value) { shininess = value; }
    void setMetalness(float value) { metalness = value; }
    void setRoughness(float value) { roughness = value; }
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

    const QVector<Vertex>& getVertices() const { return m_vertices; }
    const QVector<uint>& getIndices() const { return m_indices; }
    Material& getMaterial() { return m_material; }

    // tools
    void subdivide();
    void LaplacianSmooth(int iterations, float lambda);
    void TaubinSmooth(int iterations, float lambda, float mu);

    // utils
    void computeNormals();
    void computeTangents();
    void computeTexCoords();
    QHash<int, QSet<int>> computeAdjacencyList();
    void unifySharedVertices();

protected:
    QVector<Vertex> m_vertices;
    QVector<uint> m_indices;
    Material m_material;
private:

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    void setupMesh();
};

