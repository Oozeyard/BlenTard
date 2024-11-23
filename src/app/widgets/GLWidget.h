#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <GL/gl.h>

#include "Node.h"
#include "Model.h"
#include "Camera.h"
#include "GridOverlay.h"
#include "Context.h"
#include "Tools.h"

#include <iostream>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void highlightNode(Node *node);

public slots:
    void activateTool(Tool* tool);

signals:
    void updateNode(Node *rootNode);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void initShaders(QOpenGLShaderProgram *program, QString vertex_shader, QString fragment_shader);
    void doAction(const ActionType &actionType);
    
    void createCube();
    void createSphere();
    void loadCustomModel();

    // Shaders
    QOpenGLShaderProgram *m_program;
    QOpenGLShaderProgram *m_programGridOverlay;
    QOpenGLShaderProgram* m_selectionProgram;  // Shader for selection

    // Grid Overlay
    GridOverlay *m_gridOverlay;

    // Scene 
    Node *m_rootNode;

    // Camera 
    Camera *m_camera;
    Context *m_contextMenu;

    // Selection (Color picking)
    void initializeSelectionBuffer();
    void renderSelectionBuffer();
    int getObjectIdAtMouse(const QPoint &pos);
    QOpenGLFramebufferObject* m_selectionFBO;  // Framebuffer for selection
    GLuint m_selectionTexture;                 // Renderbuffer Object for depth testing

};



