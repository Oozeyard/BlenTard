#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_textures(std::move(textures)), VAO(new QOpenGLVertexArrayObject), VBO(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer)), EBO(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)) {
    setupMesh();
}

void Mesh::Draw(GLuint shaderProgram) {
    unsigned int diffuseNr = 1;
    unsigned int normalNr = 1;
    unsigned int metallicNr = 1;
    unsigned int roughnessNr = 1;

    for (unsigned int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = m_textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_metallic")
            number = std::to_string(metallicNr++);
        else if (name == "texture_roughness")
            number = std::to_string(roughnessNr++);

        glUniform1i(glGetUniformLocation(shaderProgram, (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }

    VAO->bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    VAO->release();

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    initializeOpenGLFunctions();
    VAO->create();
    VBO->create();
    EBO->create();

    VAO->bind();

    VBO->bind();
    VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    VBO->allocate(m_vertices.data(), m_vertices.size() * sizeof(Vertex));

    EBO->bind();
    EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    EBO->allocate(m_indices.data(), m_indices.size() * sizeof(unsigned int));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    VAO->release();
    VBO->release();
    EBO->release();
}