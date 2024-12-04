#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QtMath>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <iostream>

enum TransformMode {
    Translate,
    Rotate,
    Scale,
    None
};

class Gizmo : protected QOpenGLFunctions {
public:

    Gizmo(QOpenGLShaderProgram *program);

    void draw();

    void setMode(TransformMode mode) { transformMode = mode; }
    TransformMode getMode() { return transformMode; }

private:
    void setupGeometry();

    void drawLines();

    void drawTranslation();
    void drawArrow();

    void drawRotation();
    void drawCircle();

    void drawScale();
    void drawCube();

    void drawTriangle(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3);


    TransformMode transformMode;

    QOpenGLShaderProgram *shaderProgram;

    QOpenGLVertexArrayObject lineVao, cubeVao, arrowVao;
    QOpenGLBuffer lineVbo, cubeVbo, arrowVbo;

    QVector<GLfloat> lineVertices;
    QVector<GLfloat> cubeVertices;
    QVector<GLfloat> arrowVertices;

    QVector3D xColor = QVector3D(1.0f, 0.0f, 0.0f);
    QVector3D yColor = QVector3D(0.0f, 1.0f, 0.0f);
    QVector3D zColor = QVector3D(0.0f, 0.0f, 1.0f);

    QMatrix4x4 modelMatrix;
};
