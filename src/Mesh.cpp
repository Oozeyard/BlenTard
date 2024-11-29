#include "Mesh.h"

Mesh::Mesh(const QString& name, const QVector<Vertex>& vertices, const QVector<uint>& indices, const Material& material, Node* parent) :
    Node(name, parent),
    m_vertices(vertices),
    m_indices(indices),
    m_material(material),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    
    m_vao.create();
    m_vertexBuffer.create();
    m_indexBuffer.create();

    setupMesh();
}

Mesh::~Mesh()
{
    m_vao.destroy();
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
}

void Mesh::setupMesh()
{
    m_vao.bind();

    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertices.data(), m_vertices.size() * sizeof(Vertex));
    
    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indices.data(), m_indices.size() * sizeof(uint));
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Position

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); // Normal

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords)); // TexCoords

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent)); // Tangent

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent)); // Bitangent

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_vao.release();
}

void Mesh::draw(QOpenGLShaderProgram* program)
{
    program->bind();

    if (parent() == nullptr) {
        program->setUniformValue("model", transform.getModelMatrix());
    } else {
        program->setUniformValue("model", qobject_cast<Node*>(parent())->transform.getModelMatrix() * transform.getModelMatrix());
    }

    program->setUniformValue("objectColor", idToColor(getId()));

    program->setUniformValue("material.albedo", m_material.albedo);
    program->setUniformValue("material.specular", m_material.specular);
    program->setUniformValue("material.emissive", m_material.emissive);
    program->setUniformValue("material.shininess", m_material.shininess);
    program->setUniformValue("material.metalness", m_material.metalness);
    program->setUniformValue("material.roughness", m_material.roughness);

    QHash<QString, uint> textureCounters;
    for (int i = 0; i < m_material.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        const QString& type = m_material.textures[i].type;
        uint& counter = textureCounters[type];
        QString uniformName = QString("%1%2").arg(type).arg(++counter); // diffuse1, diffuse2, etc.

        program->setUniformValue(uniformName.toStdString().c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_material.textures[i].id);
    }

    program->setUniformValue("numDiffuseTextures", textureCounters["texture_diffuse"]);
    program->setUniformValue("numNormalTextures", textureCounters["texture_normal"]);


    m_vao.bind();
    m_vertexBuffer.bind();
    m_indexBuffer.bind();
    
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    if (m_selected) {
        program->setUniformValue("selected", true);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f); 
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    program->setUniformValue("selected", false);

    // Débind textures
    for (int i = 0; i < m_material.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0);

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_vao.release();

    program->release();
}

void Mesh::subdivide()
{
    QVector<Vertex> newVertices = m_vertices;
    QVector<uint> newIndices;

    QHash<QPair<uint, uint>, uint> midpointCache;

    auto getMidpoint = [&](uint v1Index, uint v2Index) -> uint {
        QPair<uint, uint> edge = qMakePair(qMin(v1Index, v2Index), qMax(v1Index, v2Index));
        if (midpointCache.contains(edge)) {
            return midpointCache[edge];
        }

        const Vertex& v1 = m_vertices[v1Index];
        const Vertex& v2 = m_vertices[v2Index];

        Vertex midpoint;
        midpoint.position = (v1.position + v2.position) / 2.0f;
        midpoint.normal = (v1.normal + v2.normal).normalized();
        midpoint.texCoords = (v1.texCoords + v2.texCoords) / 2.0f;
        midpoint.tangent = (v1.tangent + v2.tangent).normalized();
        midpoint.bitangent = (v1.bitangent + v2.bitangent).normalized();

        uint midpointIndex = newVertices.size();
        newVertices.append(midpoint);
        midpointCache[edge] = midpointIndex;

        return midpointIndex;
    };

    for (int i = 0; i < m_indices.size(); i += 3) {
        uint i1 = m_indices[i];
        uint i2 = m_indices[i + 1];
        uint i3 = m_indices[i + 2];

        uint m1 = getMidpoint(i1, i2);
        uint m2 = getMidpoint(i2, i3);
        uint m3 = getMidpoint(i3, i1);

        newIndices.append(i1); newIndices.append(m1); newIndices.append(m3); // Triangle 1
        newIndices.append(i2); newIndices.append(m2); newIndices.append(m1); // Triangle 2
        newIndices.append(i3); newIndices.append(m3); newIndices.append(m2); // Triangle 3
        newIndices.append(m1); newIndices.append(m2); newIndices.append(m3); // Triangle 4 (central)
    }

    m_vertices = newVertices;
    m_indices = newIndices;

    setupMesh();
}


