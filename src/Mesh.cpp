#include "Mesh.h"

Mesh::Mesh(const QString& name, const QVector<Vertex>& vertices, const QVector<uint>& indices, const Material& material, Node* parent) :
    Node(name, parent),
    m_vertices(vertices),
    m_indices(indices),
    m_material(material),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_colorBuffer(QOpenGLBuffer::VertexBuffer)
{
    initializeOpenGLFunctions();
    
    m_vao.create();
    m_vertexBuffer.create();
    m_indexBuffer.create();
    m_colorBuffer.create();

    setupMesh();
}

Mesh::~Mesh()
{
    m_vao.destroy();
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
    m_colorBuffer.destroy();
}

void Mesh::setupMesh()
{
    m_vertexColors.clear();
    for (int i = 0; i < m_vertices.size(); i++) {
        m_vertexColors.push_back(idToColor(i + 1)); // Unique color for each vertex
    }

    m_selectedVertices.resize(m_vertices.size());
    // std::fill(m_selectedVertices.begin(), m_selectedVertices.end(), false);

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
    
    m_colorBuffer.bind();
    m_colorBuffer.allocate(m_vertexColors.data(), m_vertexColors.size() * sizeof(QVector3D));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0); // Vertex color
    m_colorBuffer.release();

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

    switch (m_selectionMode) {
        case DrawSelectionMode::UseCulling:
            m_vao.bind();
            m_vertexBuffer.bind();
            m_indexBuffer.bind();
            program->setUniformValue("objectColor", QVector3D(1.0f, 1.0f, 1.0f));
            glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
            m_vertexBuffer.release();
            m_indexBuffer.release();
            m_vao.release();
            return;
        case DrawSelectionMode::Objects:
            m_vao.bind();
            m_vertexBuffer.bind();
            m_indexBuffer.bind();
            program->setUniformValue("objectColor", idToColor(getId()));
            glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
            m_vertexBuffer.release();
            m_indexBuffer.release();
            m_vao.release();
            return;
        case DrawSelectionMode::Vertices:
            glPointSize(5.0f); 
            m_vao.bind();
            program->setUniformValue("objectColor", QVector3D(0.0f, 0.0f, 0.0f));
            glDrawArrays(GL_POINTS, 0, m_vertices.size());
            m_vao.release();
            return;
        default:
            break;
    }

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
    
    if (m_selectionMode != DrawSelectionMode::Vertices) glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

    // Draw wireframe if selected
    if (m_selected) {
        program->setUniformValue("selected", true);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f); 
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } 
    program->setUniformValue("selected", false);

    // Draw vertices if in edit mode
    if (m_editMode) {
        program->setUniformValue("editMode", true);
        glPointSize(5.0f);
        for (int i = 0; i < m_vertices.size(); ++i) {
            if (m_selectedVertices[i]) {
            program->setUniformValue("objectColor", QVector3D(1.0f, 0.5f, 0.0f)); 
            } else {
            program->setUniformValue("objectColor", QVector3D(0.0f, 0.0f, 0.0f)); 
            }
            glDrawArrays(GL_POINTS, i, 1);
        }
    }
    program->setUniformValue("editMode", false);


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

void Mesh::updateVertexPosition(int index, const QVector3D& position)
{
    m_vertices[index].position = position;
    m_vertexBuffer.bind();
    m_vertexBuffer.write(index * sizeof(Vertex), &m_vertices[index], sizeof(Vertex));
    m_vertexBuffer.release();
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


