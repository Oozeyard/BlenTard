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

#include "Model.h"
#include "Camera.h"
#include "GridOverlay.h"

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

signals:
    void rootNodeCreated(Node *rootNode); // Signal pour envoyer la Node racine

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
    void drawAxisOverlay(); // Draw axis overlay    

    // Shaders
    QOpenGLShaderProgram *m_program;
    QOpenGLShaderProgram *m_programGridOverlay;

    // Grid Overlay
    GridOverlay *m_gridOverlay;

    // Scene 
    Node *m_rootNode;

    // Camera 
    Camera *m_camera;

};

