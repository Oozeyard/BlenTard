#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include "Mesh.h"
#include "Camera.h"

#include <iostream>
#include <string>


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    Mesh *m_mesh;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void initShaders(QOpenGLShaderProgram *program, std::string vertex_shader, std::string fragment_shader);
    void drawGrid(float gridSize = 10.0f, int divisions = 10.0f);
    void updateCamera();

    QOpenGLShaderProgram *m_program;

    QOpenGLVertexArrayObject m_vao; // Vertex Array Object

    // Camera 
    Camera *m_camera;

    // Transformation matrix
    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;
    QMatrix4x4 m_model;



};

