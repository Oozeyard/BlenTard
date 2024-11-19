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
    GridOverlay(QOpenGLShaderProgram *program);
    ~GridOverlay();

    void setupGrid(float size = 10.0f, float step = 1.0f);  
    void draw();

    void setEnabled(bool enabled);
    bool isEnabled() const;

private:
    QOpenGLVertexArrayObject m_gridVAO;
    QOpenGLBuffer m_gridVBO;
    QOpenGLBuffer m_gridColorVBO;

    QOpenGLShaderProgram *m_program;

    float m_gridScale { 50.0f };
    bool m_enabled { true };
};