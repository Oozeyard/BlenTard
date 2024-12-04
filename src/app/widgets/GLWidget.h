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
#include "Utils.h"
#include "Shader.h"
#include "Gizmo.h"

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
    void clearScene();

public slots:
    void activateTool(Tool* tool);
    void setCurrentNode(Node *node) { m_currentNode = node; }

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
    void createSuzanne();
    void loadCustomModel();

    // Manage transformation
    void grabNodeSelected();
    void scaleNodeSelected();
    void rotateNodeSelected();
    // QVector3D m_constraint { 1.0f, 1.0f, 1.0f };

    // Edit mode
    bool m_isEditMode { false };

    // Shaders
    Shader *m_shaderProgram;
    Shader *m_shaderGridOverlayProgram;
    Shader *m_shaderSelectionProgram;  // Shader for selection
    Shader *m_gizmoProgram;

    // Grid Overlay
    GridOverlay *m_gridOverlay;

    // Scene 
    Node *m_rootNode;
    Node *m_currentNode;

    // Camera 
    Camera *m_camera;
    Context *m_contextMenu;

    // Gizmo
    Gizmo *m_gizmo;

    // Interactive transformation
    TransformMode m_transformMode { TransformMode::None };
    Transform m_currentNodeTransform, m_lastNodeTransform;
    std::atomic<int> m_activeTransforms{0}, m_nbTotalActiveTransform{0}; // Number of active transformations

    // Selection (Color picking)
    void initializeSelectionBuffer();
    void renderSelectionBuffer();
    int getObjectIdAtMouse(const QPoint &pos);
    QOpenGLFramebufferObject* m_selectionFBO;  // Framebuffer for selection
    GLuint m_selectionTexture;                 // Renderbuffer Object for depth testing
};



