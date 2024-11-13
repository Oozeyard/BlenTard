#include "Mesh.h"

Mesh::Mesh(): m_vertexBuffer(QOpenGLBuffer::VertexBuffer), m_indexBuffer(QOpenGLBuffer::IndexBuffer) 
{
    initializeOpenGLFunctions();

    m_vao.create();
    m_vertexBuffer.create();
    m_indexBuffer.create();
    init();
}

Mesh::~Mesh() 
{
    m_vao.destroy();
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
}

void Mesh::init() 
{
    
    // Init mesh with a cube

    // cube geometry
    m_vertices = {
        // Position (X, Y, Z)      // Normale (NX, NY, NZ)
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f, // Face avant
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f, // Face arrière
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f, 0.0f, // Face gauche
        -0.5f, -0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,

         0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, // Face droite
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f, // Face dessous
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f, 0.0f, // Face dessus
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f, 0.0f
    };


    m_indices = {
        // Face avant
        0, 1, 2, 2, 3, 0,
        // Face arrière
        4, 7, 6, 6, 5, 4,
        // Face gauche
        8, 9, 10, 10, 11, 8,
        // Face droite
        12, 15, 14, 14, 13, 12,
        // Face dessous
        16, 17, 18, 18, 19, 16,
        // Face dessus
        20, 23, 22, 22, 21, 20
    };

    m_vao.bind();
    // Transfer vertex data to VBO 0
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertices.data(), m_vertices.size() * sizeof(GLfloat));

    // Transfer index data to VBO 1
    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indices.data(), m_indices.size() * sizeof(GLushort));
}

// Load mesh from OFF file
void Mesh::openOFF( std::string const & filename,
              std::vector<GLfloat> & m_vertices,
              std::vector<GLushort> & m_indices,
              bool load_normals)
{
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cerr << filename << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;
    myfile >> magic_s;

    if (magic_s != "OFF")
    {
        std::cerr << magic_s << " != OFF : We handle ONLY *.off files." << std::endl;
        myfile.close();
        throw std::runtime_error("Invalid file format");
    }

    int n_vertices, n_faces, dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    m_vertices.clear();
    m_indices.clear();

    // Read vertices (with optional normals)
    for (int v = 0; v < n_vertices; ++v)
    {
        float x, y, z;

        // Read position
        myfile >> x >> y >> z;
        m_vertices.push_back(x);
        m_vertices.push_back(y);
        m_vertices.push_back(z);

        // If load_normals is true, read normals, otherwise skip
        if (load_normals) {
            float nx, ny, nz;
            myfile >> nx >> ny >> nz;
            m_vertices.push_back(nx);
            m_vertices.push_back(ny);
            m_vertices.push_back(nz);
        }
    }

    // Read faces (indices)
    for (int f = 0; f < n_faces; ++f)
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if (n_vertices_on_face == 3)
        {
            GLuint v1, v2, v3;
            myfile >> v1 >> v2 >> v3;
            m_indices.push_back(v1);
            m_indices.push_back(v2);
            m_indices.push_back(v3);
        }
        else if (n_vertices_on_face == 4)
        {
            GLuint v1, v2, v3, v4;
            myfile >> v1 >> v2 >> v3 >> v4;
            // Break the quad into two triangles
            m_indices.push_back(v1);
            m_indices.push_back(v2);
            m_indices.push_back(v3);
            m_indices.push_back(v1);
            m_indices.push_back(v3);
            m_indices.push_back(v4);
        }
        else
        {
            std::cerr << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            throw std::runtime_error("Unsupported face format");
        }
    }

    myfile.close();
}

void Mesh::setMesh(std::string filename) 
{
    // Load mesh from OFF file
    openOFF(filename, m_vertices, m_indices, true);
    
    // Transfer vertex data to VBO 0
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertices.data(), m_vertices.size() * sizeof(GLfloat));

    // Transfer index data to VBO 1
    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indices.data(), m_indices.size() * sizeof(GLushort));
}

void Mesh::draw(QOpenGLShaderProgram *program, Camera *camera) 
{
    // Tell OpenGL which VBOs to use
    program->bind();
    m_vao.bind();
    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    program->setUniformValue("projection", camera->getProjectionMatrix());
    program->setUniformValue("view", camera->getViewMatrix());
    program->setUniformValue("model", QMatrix4x4());

    program->enableAttributeArray(0);
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));

    program->enableAttributeArray(1);
    program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, nullptr);
    
    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_vao.release();
    program->release();
}