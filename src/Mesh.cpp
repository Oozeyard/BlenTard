#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : vertices(vertices), indices(indices), textures(textures) {
    setupMesh();
}

void Mesh::Draw(GLuint shaderProgram) {
    unsigned int diffuseNr = 1;
    unsigned int normalNr = 1;
    unsigned int metallicNr = 1;
    unsigned int roughnessNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_metallic")
            number = std::to_string(metallicNr++);
        else if (name == "texture_roughness")
            number = std::to_string(roughnessNr++);

        glUniform1i(glGetUniformLocation(shaderProgram, (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    VAO.bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    VAO.release();

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    VAO.create();
    VAO.bind();

    VBO.create();
    VBO.bind();
    VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    VBO.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    EBO.create();
    EBO.bind();
    EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    EBO.allocate(indices.data(), indices.size() * sizeof(unsigned int));

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    f->glEnableVertexAttribArray(3);
    f->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    f->glEnableVertexAttribArray(4);
    f->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    VAO.release();
    VBO.release();
    EBO.release();
}