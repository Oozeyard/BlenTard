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
#include <QApplication>
#include <QCoreApplication>
#include <GL/gl.h>

#include "Node.h"
#include "Model.h"
#include "Camera.h"
#include "GridOverlay.h"
#include "Context.h"
#include "Tools.h"
#include "Shader.h"

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
    void NodeSelected(Node *node);
    
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
    void doAction(const ActionType &actionType);
    
    void createCube();
    void createSphere();
    void loadCustomModel();

    // Manage transformation
    void grabNodeSelected();
    bool m_isGrabbing { false };
    bool m_isConstrained { false };
    QVector3D m_constraint { 1.0f, 1.0f, 1.0f };
    QVector3D rayPlaneIntersection(const QVector3D& rayOrigin, const QVector3D& rayDirection, const QVector3D& planeOrigin, const QVector3D& planeNormal);
    void mouseToRay(const QPoint& mousePos, QVector3D& rayOrigin, QVector3D& rayDirection);

    // Shaders
    Shader *m_shaderProgram;
    Shader *m_shaderGridOverlayProgram;
    Shader *m_shaderSelectionProgram;  // Shader for selection

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



