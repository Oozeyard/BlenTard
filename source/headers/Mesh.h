#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <iostream>
#include <fstream>
#pragma once

class Mesh {
public:
    Mesh();
    ~Mesh();

    void draw(QOpenGLShaderProgram *program);
    void setMesh(std::string filename);
    void openOFF( std::string const & filename,
              std::vector<GLfloat> & m_vertices,
              std::vector<GLushort> & m_indices,
              bool load_normals = true );

private:
    void init();

    std::vector<GLushort> m_indices;
    std::vector<GLfloat> m_vertices;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
};