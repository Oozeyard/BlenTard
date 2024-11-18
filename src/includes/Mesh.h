#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "Node.h"
#include "Camera.h"

#include <iostream>
#include <fstream>

class Mesh : public Node, protected QOpenGLFunctions 
{
    Q_OBJECT
public:
    explicit Mesh(const QString& name, Node *parent = nullptr);
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

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
};