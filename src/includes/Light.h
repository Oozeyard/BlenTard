#pragma once

#include "Node.h"
#include "Shader.h"
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObjectFormat>

class Light : public Node {
    Q_OBJECT

public:
    enum LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    explicit Light(const QString& name, LightType type = DIRECTIONAL, QObject* parent = nullptr);
    ~Light();

    // Setters and Getters
    void setColor(const QVector3D& color) { m_color = color; }
    QVector3D getColor() const { return m_color; }

    void setIntensity(float intensity) { m_intensity = intensity; }
    float getIntensity() const { return m_intensity; }

    void setType(LightType type) { m_type = type; }
    LightType getType() const { return m_type; }

    GLuint getShadowMapTexture() const { return m_shadowTexture; }

    QMatrix4x4  getLightSpaceMatrix() const { return m_lightProjection * m_lightView; };

    // Shadow map
    void initializeShadowMap(int width, int height);
    void renderShadowMap(Shader* shader, Node* root = nullptr);

    void draw(QOpenGLShaderProgram* program) override;

private:
    LightType m_type;
    QVector3D m_color;
    float m_intensity;

    void printShadowMap();

    // Shadow map resources
    QOpenGLFramebufferObject* m_shadowFBO;
    GLuint m_shadowTexture;
    int m_shadowWidth;
    int m_shadowHeight;

    QMatrix4x4 m_lightProjection;
    QMatrix4x4 m_lightView;
};