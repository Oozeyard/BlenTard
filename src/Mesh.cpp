#include "Mesh.h"

Mesh::Mesh(const QString& name, const QVector<Vertex>& vertices, const QVector<uint>& indices, const QVector<Texture>& textures, Node* parent) :
    Node(name, parent),
    m_vertices(vertices),
    m_indices(indices),
    m_textures(textures),
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

    QHash<QString, uint> textureCounters;

    if (parent() == nullptr) {
        program->setUniformValue("model", transform.getModelMatrix());
    } else {
        program->setUniformValue("model", qobject_cast<Node*>(parent())->transform.getModelMatrix() * transform.getModelMatrix());
    }

    for (int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        const QString& type = m_textures[i].type;
        uint& counter = textureCounters[type]; 
        QString uniformName = QString("%1%2").arg(type).arg(++counter); // diffuse1, diffuse2, specular1, specular2, etc.

        program->setUniformValue(uniformName.toStdString().c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }

    if (textureCounters.contains("texture_diffuse")) {
        program->setUniformValue("numDiffuseTextures", textureCounters["texture_diffuse"]);
    }
    if (textureCounters.contains("texture_normal")) {
        program->setUniformValue("numNormalTextures", textureCounters["texture_normal"]);
    }

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

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_vao.release();

    glActiveTexture(GL_TEXTURE0);
    program->release();
}

