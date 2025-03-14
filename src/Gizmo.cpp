#include "Gizmo.h"

Gizmo::Gizmo(QOpenGLShaderProgram *program) : transformMode(None), shaderProgram(program)  {
    initializeOpenGLFunctions();
    lineVao.create();
    lineVbo.create();
    cubeVao.create();
    cubeVbo.create();
    arrowVao.create();
    arrowVbo.create();
    setupGeometry();
}

void Gizmo::setupGeometry() {
    lineVertices = {
        // X-axis (red)
        0.0f, 0.0f, 0.0f,// Origin
        1.0f, 0.0f, 0.0f, // End

        // Y-axis (green)
        0.0f, 0.0f, 0.0f, // Origin
        0.0f, 1.0f, 0.0f, // End

        // Z-axis (blue)
        0.0f, 0.0f, 0.0f, // Origin
        0.0f, 0.0f, 1.0f // End
    };

    lineVao.bind();

    lineVbo.bind();
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(GLfloat), lineVertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

    lineVbo.release();
    lineVao.release();

    // Cube
    cubeVertices = {
        -0.05f, -0.05f,  0.05f,
         0.05f, -0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f,  0.05f,

        -0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
        -0.05f,  0.05f, -0.05f,

        -0.05f, -0.05f,  0.05f,
        -0.05f, -0.05f, -0.05f,
        -0.05f,  0.05f, -0.05f,
        -0.05f,  0.05f,  0.05f,

         0.05f, -0.05f,  0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
         0.05f,  0.05f,  0.05f,

        -0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f, -0.05f,  0.05f,
        -0.05f, -0.05f,  0.05f,

        -0.05f,  0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
         0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f,  0.05f
    };

    cubeVao.bind();
    cubeVbo.bind();
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(GLfloat), cubeVertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

    cubeVbo.release();
    cubeVao.release();

    // Arrow
    arrowVertices = {
        -0.05f, 0.0f, 0.85f,
        0.05f, 0.0f, 0.85f,
        0.0f,  0.05f, 0.85f,

        -0.05f, 0.0f, 0.85f,
        0.0f, -0.05f, 0.85f,
        0.05f, 0.0f, 0.85f,

        0.0f,  0.0f, 1.0f,
        -0.05f, 0.0f, 0.85f,
        0.05f, 0.0f, 0.85f,

        0.0f,  0.0f, 1.0f,
        0.05f, 0.0f, 0.85f,
        0.0f,  0.05f, 0.85f,

        0.0f,  0.0f, 1.0f,
        0.0f,  0.05f, 0.85f,
        -0.05f, 0.0f, 0.85f,

        0.0f,  0.0f, 1.0f,
        -0.05f, 0.0f, 0.85f,
        0.0f, -0.05f, 0.85f
    };


    arrowVao.bind();
    arrowVbo.bind();
    glBufferData(GL_ARRAY_BUFFER, arrowVertices.size() * sizeof(GLfloat), arrowVertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

    arrowVbo.release();
    arrowVao.release();
    
}

void Gizmo::draw() {
    shaderProgram->bind();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    

    switch (transformMode) {
    case Translate:
        drawTranslation();
        break;
    case Rotate:
        drawRotation();
        break;
    case Scale:
        drawScale();
        break;
    case None:
        break;
    default:
        break;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    shaderProgram->release();
}


void Gizmo::drawLines() {
    QMatrix4x4 model;
    model.setToIdentity();
    shaderProgram->setUniformValue("model", model);

    lineVao.bind();
    shaderProgram->setUniformValue("color", QVector3D(1.0f, 0.0f, 0.0f));  // Rouge
    glDrawArrays(GL_LINES, 0, 2);  // X

    shaderProgram->setUniformValue("color", QVector3D(0.0f, 1.0f, 0.0f));  // Vert
    glDrawArrays(GL_LINES, 2, 2);  // Y

    shaderProgram->setUniformValue("color", QVector3D(0.0f, 0.0f, 1.0f));  // Bleu
    glDrawArrays(GL_LINES, 4, 2);  // Z
    lineVao.release();
}

void Gizmo::drawTranslation() {
    QMatrix4x4 modelMatrix;

    drawLines();

    // Draw arrow for each axis
    for (int axis = 0; axis < 3; ++axis) {
        modelMatrix.setToIdentity();

        switch (axis) {
        case 0: // X
            modelMatrix.rotate(90.0f, 0.0f, 1.0f, 0.0f);
            shaderProgram->setUniformValue("color", QVector3D(1.0f, 0.0f, 0.0f));
            break;
        case 1: // Y
            modelMatrix.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
            shaderProgram->setUniformValue("color", QVector3D(0.0f, 1.0f, 0.0f));
            break;
        case 2: // Z
            shaderProgram->setUniformValue("color", QVector3D(0.0f, 0.0f, 1.0f));
            break;
        }

        shaderProgram->setUniformValue("model", modelMatrix);

        drawArrow();
    }
}

void Gizmo::drawArrow() {
    arrowVao.bind();

    glDrawArrays(GL_TRIANGLES, 0, arrowVertices.size());

    arrowVao.release();
}


void Gizmo::drawRotation() {
    QMatrix4x4 modelMatrix;

    // Draw circle for each axis
    for (int axis = 0; axis < 3; ++axis) {
        modelMatrix.setToIdentity();

        switch (axis) {
        case 0: // X
            modelMatrix.rotate(90.0f, 0.0f, 1.0f, 0.0f);
            shaderProgram->setUniformValue("color", QVector3D(1.0f, 0.0f, 0.0f));
            break;
        case 1: // Y
            modelMatrix.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
            shaderProgram->setUniformValue("color", QVector3D(0.0f, 1.0f, 0.0f));
            break;
        case 2: // Z
            shaderProgram->setUniformValue("color", QVector3D(0.0f, 0.0f, 1.0f));
            break;
        }

        shaderProgram->setUniformValue("model", modelMatrix);

        drawCircle();
    }
}

void Gizmo::drawCircle() {
    const int segments = 64;
    const float radius = 1.0f;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
}

void Gizmo::drawScale() {
    QMatrix4x4 modelMatrix;

    drawLines();

    // Draw cube for each axis
    for (int axis = 0; axis < 3; ++axis) {
        modelMatrix.setToIdentity();

        switch (axis) {
        case 0: // X
            modelMatrix.translate(1.0f, 0.0f, 0.0f);
            shaderProgram->setUniformValue("color", QVector3D(1.0f, 0.0f, 0.0f));
            break;
        case 1: // Y
            modelMatrix.translate(0.0f, 1.0f, 0.0f);
            shaderProgram->setUniformValue("color", QVector3D(0.0f, 1.0f, 0.0f));
            break;
        case 2: // Z
            modelMatrix.translate(0.0f, 0.0f, 1.0f);
            shaderProgram->setUniformValue("color", QVector3D(0.0f, 0.0f, 1.0f));
            break;
        }

        shaderProgram->setUniformValue("model", modelMatrix);

        drawCube();
    }
}

void Gizmo::drawCube() {
    cubeVao.bind();

    glDrawArrays(GL_QUADS, 0, cubeVertices.size());

    cubeVao.release();
}
