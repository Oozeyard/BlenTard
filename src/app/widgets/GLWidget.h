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
#include <QRect>
// #include <GL/gl.h>

#include "Node.h"
#include "Model.h"
#include "Camera.h"
#include "GridOverlay.h"
#include "Context.h"
#include "Tools.h"
#include "Utils.h"
#include "Shader.h"
#include "Gizmo.h"
#include "GLPainter.h"
#include "Light.h"

#include <iostream>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    enum ShadingMode {
        SHADER_TEXTURE,
        SHADER_RENDER,
        SHADER_SOLID,
        SHADER_WIRE
    };

    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void highlightNode(Node *node);
    void clearScene();

public slots:
    void activateTool(Tool* tool);
    void setCurrentNode(Node *node) { m_currentNode = node; }
    void unselectNodes();
    void keyPressed(QKeyEvent *event);

signals:
    void updateNode(Node *rootNode);
    void NodeSelected(Node *node);
    
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void doAction(const ActionType &actionType);
    
    void createCube();
    void createSphere();
    void createSuzanne();
    void loadCustomModel();

    void createLight(Light::LightType type);

    // Manage transformation
    void grabNodeSelected();
    void scaleNodeSelected();
    void rotateNodeSelected();

    // Manage vertices transformation
    void grabVerticesSelected();
    void scaleVerticesSelected();
    void rotateVerticesSelected();

    // QVector3D m_constraint { 1.0f, 1.0f, 1.0f };
    
    ShadingMode m_shading;

    // Edit mode
    bool m_isEditMode { false };

    // Shaders
    Shader *m_shaderSolid { nullptr }; // Solid color
    Shader *m_shaderTexture { nullptr }; // Texture
    Shader *m_shaderRender { nullptr }; // Render (w/ light)

    Shader *m_shaderShadowMap { nullptr }; // Shadow map
    Shader *m_shaderSelected { nullptr }; // Selected  shader

    Shader *m_shaderGridOverlayProgram { nullptr };
    Shader *m_shaderSelectionProgram { nullptr };  // Shader for selection
    Shader *m_gizmoProgram { nullptr };

    // Grid Overlay
    GridOverlay *m_gridOverlay;

    // Scene 
    Node *m_rootNode;
    Node *m_currentNode { nullptr };

    // Camera 
    Camera *m_camera;
    Context *m_contextMenu { new Context(this) };

    // Gizmo
    Gizmo *m_gizmo;

    // Interactive transformation
    TransformMode m_transformMode { TransformMode::None };
    Transform m_initialNodeTransform, m_lastNodeTransform;
    std::atomic<int> m_activeTransforms{0}, m_nbTotalActiveTransform{0}; // Number of active transformations

    // Selection (Color picking) 
    bool m_isShitKeyPressed { false }; 
    GLPainter *m_glPainter;
    QRect m_selectedRect;
    void initializeSelectionBuffer();
    void renderNodeSelectionBuffer();
    void renderVerticesSelectionBuffer();
    QOpenGLFramebufferObject* m_selectionFBO;  // Framebuffer for selection
    GLuint m_selectionTexture;                 // Renderbuffer Object for depth testing

    int getObjectIdAtMouse(const QPoint &pos);
    QVector<int> getObjectsIdInRect(const QRect &rect);

    QVector<int> m_selectedVertexIndices;
};



