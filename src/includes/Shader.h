#pragma once

#include <QOpenGLShaderProgram>
#include <QString>

class Shader : public QOpenGLShaderProgram
{
public:
    Shader(const QString& vertexShaderPath, const QString& fragmentShaderPath);
    ~Shader();

    void bind();
    void release();

private:
    void initShaders(const QString& vertexShaderPath, const QString& fragmentShaderPath);
};
