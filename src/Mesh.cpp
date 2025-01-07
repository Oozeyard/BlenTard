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

uint Mesh::textureFromFile(const QString& path, const QString& directory)
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
        midpoint.texCoords = (v1.texCoords + v2.texCoords) / 2.0f;

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

    computeNormals();
    computeTangents();

    setupMesh();
}

void Mesh::LaplacianSmooth(int iterations = 1, float lambda = 0.5f) {

    if (m_vertices.isEmpty() || m_indices.isEmpty()) {
        std::cerr << "Mesh is empty, cannot smooth.\n";
        return;
    }
    
    for (int i = 0; i < iterations; ++i) {
        // neighbors
        QHash<int, QSet<int>> adjacencyList = computeAdjacencyList();

        // Calcul des nouvelles positions
        QVector<QVector3D> newPositions(m_vertices.size());
        for (int i = 0; i < m_vertices.size(); ++i) {
            QVector3D sum(0, 0, 0);
            for (int neighbor : adjacencyList[i]) {
                sum += m_vertices[neighbor].position;
            }

            if (!adjacencyList[i].isEmpty()) {
                sum /= static_cast<float>(adjacencyList[i].size());
            }

            newPositions[i] = m_vertices[i].position + lambda * (sum - m_vertices[i].position);
        }

        // Mise à jour des positions
        for (int i = 0; i < m_vertices.size(); ++i) {
            m_vertices[i].position = newPositions[i];
        }
    }

    // Recalculer les normales et tangentes après le lissage
    computeNormals();
    computeTangents();

    // Reconfigurer les buffers OpenGL
    setupMesh();
}

void Mesh::TaubinSmooth(int iterations = 1, float lambda = 0.5f, float mu = 0.53f) {

    if (m_vertices.isEmpty() || m_indices.isEmpty()) {
        std::cerr << "Mesh is empty, cannot smooth.\n";
        return;
    }

    for (int i = 0; i < iterations; ++i) {
        // neighbors
        QHash<int, QSet<int>> adjacencyList = computeAdjacencyList();

        // Calcul des nouvelles positions
        QVector<QVector3D> newPositions(m_vertices.size());
        for (int i = 0; i < m_vertices.size(); ++i) {
            QVector3D sum(0, 0, 0);
            for (int neighbor : adjacencyList[i]) {
                sum += m_vertices[neighbor].position;
            }

            if (!adjacencyList[i].isEmpty()) {
                sum /= static_cast<float>(adjacencyList[i].size());
            }

            newPositions[i] = m_vertices[i].position + lambda * (sum - m_vertices[i].position);
        }

        // Mise à jour des positions
        for (int i = 0; i < m_vertices.size(); ++i) {
            m_vertices[i].position = newPositions[i];
        }

        // Calcul des nouvelles positions
        QVector<QVector3D> newPositions2(m_vertices.size());
        for (int i = 0; i < m_vertices.size(); ++i) {
            QVector3D sum(0, 0, 0);
            for (int neighbor : adjacencyList[i]) {
                sum += m_vertices[neighbor].position;
            }

            if (!adjacencyList[i].isEmpty()) {
                sum /= static_cast<float>(adjacencyList[i].size());
            }

            newPositions2[i] = m_vertices[i].position + mu * (sum - m_vertices[i].position);
        }

        // Mise à jour des positions
        for (int i = 0; i < m_vertices.size(); ++i) {
            m_vertices[i].position = newPositions2[i];
        }
    }

    // Recalculer les normales et tangentes après le lissage
    computeNormals();
    computeTangents();

    // Reconfigurer les buffers OpenGL
    setupMesh();
}

void Mesh::computeNormals() {

    // Reset normals
    for (auto& vertex : m_vertices) {
        vertex.normal = QVector3D(0.0f, 0.0f, 0.0f);
    }

    // Compute
    for (int i = 0; i < m_indices.size(); i += 3) {
        uint i1 = m_indices[i];
        uint i2 = m_indices[i + 1];
        uint i3 = m_indices[i + 2];

        QVector3D v1 = m_vertices[i2].position - m_vertices[i1].position;
        QVector3D v2 = m_vertices[i3].position - m_vertices[i1].position;

        QVector3D normal = QVector3D::crossProduct(v1, v2).normalized();

        m_vertices[i1].normal += normal;
        m_vertices[i2].normal += normal;
        m_vertices[i3].normal += normal;
    }

    for (auto& vertex : m_vertices) {
        vertex.normal.normalize();
    }
}

void Mesh::computeTangents() {

    // Reset tangents and bitangents
    for (auto& vertex : m_vertices) {
        vertex.tangent = QVector3D(0.0f, 0.0f, 0.0f);
        vertex.bitangent = QVector3D(0.0f, 0.0f, 0.0f);
    }

    // Compute
    for (int i = 0; i < m_indices.size(); i += 3) {
        uint i1 = m_indices[i];
        uint i2 = m_indices[i + 1];
        uint i3 = m_indices[i + 2];

        const Vertex& v1 = m_vertices[i1];
        const Vertex& v2 = m_vertices[i2];
        const Vertex& v3 = m_vertices[i3];

        QVector3D edge1 = v2.position - v1.position;
        QVector3D edge2 = v3.position - v1.position;

        QVector2D deltaUV1 = v2.texCoords - v1.texCoords;
        QVector2D deltaUV2 = v3.texCoords - v1.texCoords;

        float f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
        if (!std::isfinite(f)) {
            f = 0.0f; // Prevent NaN
        }

        QVector3D tangent = f * (deltaUV2.y() * edge1 - deltaUV1.y() * edge2);
        QVector3D bitangent = f * (-deltaUV2.x() * edge1 + deltaUV1.x() * edge2);

        m_vertices[i1].tangent += tangent;
        m_vertices[i1].bitangent += bitangent;

        m_vertices[i2].tangent += tangent;
        m_vertices[i2].bitangent += bitangent;

        m_vertices[i3].tangent += tangent;
        m_vertices[i3].bitangent += bitangent;
    }

    for (auto& vertex : m_vertices) {
        vertex.tangent.normalize();
        vertex.bitangent.normalize();
    }
}

void Mesh::computeTexCoords() {
   if (m_vertices.isEmpty()) {
        std::cerr << "Mesh::computeTexCoords - No vertices to process." << std::endl;
        return;
    }

    // Déterminer les limites du maillage pour normaliser les coordonnées
    QVector3D minCoords(FLT_MAX, FLT_MAX, FLT_MAX);
    QVector3D maxCoords(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (const auto& vertex : m_vertices) {
        minCoords.setX(std::min(minCoords.x(), vertex.position.x()));
        minCoords.setY(std::min(minCoords.y(), vertex.position.y()));
        minCoords.setZ(std::min(minCoords.z(), vertex.position.z()));

        maxCoords.setX(std::max(maxCoords.x(), vertex.position.x()));
        maxCoords.setY(std::max(maxCoords.y(), vertex.position.y()));
        maxCoords.setZ(std::max(maxCoords.z(), vertex.position.z()));
    }

    QVector3D range = maxCoords - minCoords;

    if (range.length() < 1e-6) {
        std::cerr << "Mesh::computeTexCoords - Mesh dimensions are too small." << std::endl;
        return;
    }

    // Assigner des coordonnées UV à chaque sommet
    for (auto& vertex : m_vertices) {
        // Normalisation des coordonnées en [0, 1]
        float u = (vertex.position.x() - minCoords.x()) / range.x();
        float v = (vertex.position.y() - minCoords.y()) / range.y();

        // Ajustement pour tenir compte d'autres dimensions si nécessaire
        vertex.texCoords = QVector2D(u, v);
    }
}

QHash<int, QSet<int>> Mesh::computeAdjacencyList() {
    QHash<int, QSet<int>> adjacencyList;

    for (int i = 0; i < m_indices.size(); i += 3) {
        int v0 = m_indices[i];
        int v1 = m_indices[i + 1];
        int v2 = m_indices[i + 2];

        adjacencyList[v0].insert(v1);
        adjacencyList[v0].insert(v2);
        adjacencyList[v1].insert(v0);
        adjacencyList[v1].insert(v2);
        adjacencyList[v2].insert(v0);
        adjacencyList[v2].insert(v1);
    }

    return adjacencyList; 
}

void Mesh::unifySharedVertices() {
    QHash<QVector3D, int> uniqueVertices;
    QVector<Vertex> newVertices;
    QVector<uint> newIndices;

    for (int i = 0; i < m_indices.size(); ++i) {
        const QVector3D& position = m_vertices[m_indices[i]].position;

        if (uniqueVertices.contains(position)) {
            newIndices.push_back(uniqueVertices[position]);
        } else {
            int newIndex = newVertices.size();
            uniqueVertices[position] = newIndex;

            newVertices.push_back(m_vertices[m_indices[i]]);
            newIndices.push_back(newIndex);
        }
    }

    m_vertices = newVertices;
    m_indices = newIndices;
    computeNormals();
    computeTangents();

    setupMesh();
}

void Mesh::edgeCollapse(float percentage) {

    if (m_indices.size() % 3 != 0) {
        qWarning() << "Invalid mesh: indices are not divisible by 3.";
        return;
    }

    // Unify shared vertices before collapsing edges
    unifySharedVertices();

    QHash<int, QSet<int>> adjacencyList = computeAdjacencyList();

    // Number of faces before edge collapse
    int previousFaceCount = m_indices.size() / 3;
    while (m_indices.size() / 3 > previousFaceCount * percentage) {

        float minError = FLT_MAX;
        QPair<int, int> edgeToCollapse;

        // Find the edge with the smallest error
        for (auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
            int v1 = it.key();
            const QSet<int>& neighbors = it.value();

            for (int v2 : neighbors) {
                float error = (m_vertices[v1].position - m_vertices[v2].position).lengthSquared();
                if (error < minError) {
                    minError = error;
                    edgeToCollapse = {v1, v2};
                }
            }
        }

        // Collapse
        int v1 = edgeToCollapse.first;
        int v2 = edgeToCollapse.second;

        QVector3D newPosition = (m_vertices[v1].position + m_vertices[v2].position) / 2;
        m_vertices[v1].position = newPosition;

        // Update indices
        for (int i = 0; i < m_indices.size(); i += 3) {
            uint& idx1 = m_indices[i];
            uint& idx2 = m_indices[i + 1];
            uint& idx3 = m_indices[i + 2];

            if (idx1 == v2) idx1 = v1;
            if (idx2 == v2) idx2 = v1;
            if (idx3 == v2) idx3 = v1;

            if (idx1 == idx2 || idx2 == idx3 || idx3 == idx1) {
                m_indices.remove(i, 3);
                i -= 3;
            }
        }

        adjacencyList.remove(v2);
        QSet<int>& neighborsV1 = adjacencyList[v1];
        QSet<int>& neighborsV2 = adjacencyList[v2];
        neighborsV1.unite(neighborsV2);
        neighborsV1.remove(v1);

        for (auto& neighbors : adjacencyList) {
            neighbors.remove(v2);
            if (neighbors.contains(v1)) {
                neighbors.insert(v1);
            }
        }

        computeNormals();
    }

    qDebug() << "Mesh simplified to" << m_indices.size() / 3 << "faces.";
    setupMesh();
}


