#include "Light.h"
#include "stb_image_write.h"

Light::Light(const QString& name, LightType type, QObject* parent)
    : Node(name, parent),
      m_type(type),
      m_color(1.0f, 1.0f, 1.0f),
      m_intensity(1.0f),
      m_shadowFBO(nullptr),
      m_shadowTexture(0),
      m_shadowWidth(1024),
      m_shadowHeight(1024) {
    initializeShadowMap(m_shadowWidth, m_shadowHeight);
}

Light::~Light() {
    delete m_shadowFBO;
}

void Light::initializeShadowMap(int width, int height) {
    m_shadowWidth = width;
    m_shadowHeight = height;

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    format.setTextureTarget(GL_TEXTURE_2D);

    m_shadowFBO = new QOpenGLFramebufferObject(width, height, format);
    m_shadowTexture = m_shadowFBO->texture();
}

void Light::renderShadowMap(Shader* shader, Node* root) {
    if (!m_shadowFBO) return;

    // Bind the shadow framebuffer object
    m_shadowFBO->bind();

    // Clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, m_shadowWidth, m_shadowHeight);

    // Set the light's view and projection matrices
    if (m_type == DIRECTIONAL) {
        // Directional light uses orthographic projection
        m_lightProjection.setToIdentity();
        m_lightProjection.ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
        m_lightView.setToIdentity();
        m_lightView.lookAt(-getTransform().position, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    } else if (m_type == POINT) {
        // Point light uses perspective projection
        m_lightProjection.setToIdentity();
        m_lightProjection.perspective(90.0f, 1.0f, 0.1f, 100.0f);
        m_lightView.setToIdentity();
        m_lightView.lookAt(-getTransform().position, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    } else if (m_type == SPOT) {
        // Spot light uses perspective projection with a narrower field of view
        m_lightProjection.setToIdentity();
        m_lightProjection.perspective(45.0f, 1.0f, 0.1f, 100.0f);
        m_lightView.setToIdentity();
        m_lightView.lookAt(-getTransform().position, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    }

    shader->bind();
    shader->setUniformValue("lightSpaceMatrix", getLightSpaceMatrix());

    // Render the scene here for shadow map generation
    for (Node* child : root->getChildren()) {
        qDebug() << "Rendering shadow map for child:" << child->getName();
        child->draw(shader);
    }

    glBindTexture(GL_TEXTURE_2D, m_shadowTexture);
    qDebug() << "Shadow texture ID:" << m_shadowTexture;

    printShadowMap();

    m_shadowFBO->release();
}

void Light::draw(QOpenGLShaderProgram* program) {
    // Pass light properties to the shader
    program->setUniformValue("light.position", transform.position);
    program->setUniformValue("light.color", m_color);
    program->setUniformValue("light.intensity", m_intensity);
    program->setUniformValue("lightSpaceMatrix", getLightSpaceMatrix());
    program->setUniformValue("shadowMap", 1);

    // Bind the shadow map texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_shadowTexture);
}

void Light::printShadowMap() {
    std::vector<float> shadowData(m_shadowWidth * m_shadowHeight);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, shadowData.data());

    // Convert depth values to a format suitable for saving as an image
    std::vector<unsigned char> imageData(m_shadowWidth * m_shadowHeight);
    for (int i = 0; i < m_shadowWidth * m_shadowHeight; ++i) {
        imageData[i] = static_cast<unsigned char>(shadowData[i] * 255.0f);
    }

    stbi_write_png("shadow_map.png", m_shadowWidth, m_shadowHeight, 1, imageData.data(), m_shadowWidth);
    
}
