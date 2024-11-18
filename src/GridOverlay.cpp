#include "GridOverlay.h"

GridOverlay::GridOverlay() : 
    m_gridVBO(QOpenGLBuffer::VertexBuffer)
{
    initializeOpenGLFunctions();

    m_gridVAO.create();
    m_gridVBO.create();
    m_gridColorVBO.create();
    setupGrid(m_gridScale, 1.0f);
}

GridOverlay::~GridOverlay()
{
    m_gridVAO.destroy();
    m_gridVBO.destroy();
    m_gridColorVBO.destroy();
}

void GridOverlay::setupGrid(float size, float step)
{
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> colors;
    float subStep = step / 2;
    bool toggle = false;
    for (float i = -size; i <= size; i += subStep)
    {
        vertices.push_back(QVector3D(i, -size, 0.0f));
        vertices.push_back(QVector3D(i, size, 0.0f));
        vertices.push_back(QVector3D(-size, i, 0.0f));
        vertices.push_back(QVector3D(size, i, 0.0f));
        if (i == 0) {
            colors.push_back(QVector3D(0, 1, 0));
            colors.push_back(QVector3D(0, 1, 0));
            colors.push_back(QVector3D(1, 0, 0));
            colors.push_back(QVector3D(1, 0, 0));
        } else if (toggle) {
            colors.push_back(QVector3D(0.4, 0.4, 0.4));
            colors.push_back(QVector3D(0.4, 0.4, 0.4));
            colors.push_back(QVector3D(0.4, 0.4, 0.4));
            colors.push_back(QVector3D(0.4, 0.4, 0.4));
        } else {
            colors.push_back(QVector3D(0.6, 0.6, 0.6));
            colors.push_back(QVector3D(0.6, 0.6, 0.6));
            colors.push_back(QVector3D(0.6, 0.6, 0.6));
            colors.push_back(QVector3D(0.6, 0.6, 0.6));
        }
        toggle = !toggle;
    }

    m_gridVAO.bind();
    m_gridVBO.bind();
    m_gridVBO.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

    m_gridColorVBO.bind();
    m_gridColorVBO.allocate(colors.data(), colors.size() * sizeof(QVector3D));

}

void GridOverlay::draw(QOpenGLShaderProgram *program)
{
    program->bind();
    m_gridVAO.bind();

    program->enableAttributeArray(0);
    m_gridVBO.bind();
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    program->enableAttributeArray(1);
    m_gridColorVBO.bind();
    program->setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawArrays(GL_LINES, 0, m_gridVBO.size() / sizeof(QVector3D));

    m_gridVBO.release();
    m_gridColorVBO.release();
    m_gridVAO.release();
    program->release();
}