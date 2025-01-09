#include "Shader.h"

Shader::Shader(const QString& vertexShaderPath, const QString& fragmentShaderPath)
{
    initShaders(vertexShaderPath, fragmentShaderPath);
}

void Shader::initShaders(const QString& vertexShaderPath, const QString& fragmentShaderPath)
{
    if (!addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath.toStdString().c_str())) {
        qWarning() << "Error when compiling the vertex shader:" << this->log();
    }

    if (!addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath.toStdString().c_str())) {
        qWarning() << "Error when compiling the fragment shader:" << this->log();
    }

    if (!link()) {
        qWarning() << "Error when linking the shader program:" << this->log();
    }
}

void Shader::bind()
{
    QOpenGLShaderProgram::bind();
}

void Shader::release()
{
    QOpenGLShaderProgram::release();
}