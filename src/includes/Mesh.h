#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "Camera.h"

#include <iostream>
#include <fstream>

class Mesh : protected QOpenGLFunctions 
{
public:
    Mesh();
    ~Mesh();

    void draw(QOpenGLShaderProgram *program, Camera *camera);
    void setMesh(std::string filename);
    void openOFF( std::string const & filename,
              std::vector<GLfloat> & m_vertices,
              std::vector<GLushort> & m_indices,
              bool load_normals = true );

private:
    void init();

    std::vector<GLushort> m_indices;
    std::vector<GLfloat> m_vertices;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
};