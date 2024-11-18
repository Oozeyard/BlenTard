#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector3D>

#include <iostream>
#include <vector>
#include <cmath>


class GridOverlay : protected QOpenGLFunctions 
{
public:
    GridOverlay();
    ~GridOverlay();

    void setupGrid(float size = 10.0f, float step = 1.0f);  
    void draw(QOpenGLShaderProgram *program);

private:
    QOpenGLVertexArrayObject m_gridVAO;
    QOpenGLBuffer m_gridVBO;
    QOpenGLBuffer m_gridColorVBO;

    float m_gridScale { 50.0f };
};