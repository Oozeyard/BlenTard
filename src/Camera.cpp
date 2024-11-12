#include "Camera.h"

Camera::Camera(QObject *parent)
    : QObject(parent),
      m_position(0.0f, 5.0f, 10.0f),
      m_target(0.0f, 0.0f, 0.0f),
      m_up(0.0f, 1.0f, 0.0f)
{
    updateCamera();
}

Camera::Camera(QVector3D position, QVector3D target, QVector3D up, QObject *parent)
    : QObject(parent),
      m_position(position),
      m_target(target),
      m_up(up)
{
    updateCamera();
}

Camera::~Camera() {}

void Camera::updateCamera()
{
    m_rotation = QQuaternion::fromEulerAngles(m_pitch, m_yaw, m_roll);
}

void Camera::computeView(QMatrix4x4 &view, QMatrix4x4 &projection) 
{ 
    projection.setToIdentity();  
    projection.perspective(m_fov, m_aspect, m_near, m_far);

    view.setToIdentity();
    view.lookAt(m_position, m_target, m_up);
}

void Camera::keyPressEvent(QKeyEvent *event)
{
    std::cout << "Key pressed: " << event->key() << std::endl;

}

void Camera::mousePressEvent(QMouseEvent *event)
{   
    std::cout << "Mouse pressed" << std::endl;
    m_lastMousePos = event->pos();
    m_mousePressed = true;
}

void Camera::mouseMoveEvent(QMouseEvent *event)
{
    /*if (!m_mousePressed) return;

    QPoint currentMousePos = event->pos();
    float xOffset = currentMousePos.x() - m_lastMousePos.x();
    float yOffset = m_lastMousePos.y() - currentMousePos.y(); // Inverted since y-coordinates go down in Qt
    m_lastMousePos = currentMousePos;

    if (event->buttons() & Qt::RightButton) {
        // Orbit around the target
        xOffset *= m_mouseSensitivity;
        yOffset *= m_mouseSensitivity;

        m_yaw += xOffset;
        m_pitch += yOffset;

        // Clamp pitch
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;

        updateCameraVectors();
    }
    else if (event->buttons() & Qt::MiddleButton) {
        // Pan the camera
        float panSensitivity = 0.005f;
        xOffset *= panSensitivity;
        yOffset *= panSensitivity;

        QVector3D forward = (m_target - m_position).normalized();
        QVector3D right = QVector3D::crossProduct(forward, m_up).normalized();
        QVector3D up = QVector3D::crossProduct(right, forward).normalized();

        m_position -= right * xOffset + up * yOffset;
        m_target -= right * xOffset + up * yOffset;
    }*/


}

void Camera::wheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y() / 120.0f; // Normalize to -1, 0, or 1
    float distance = (m_position - m_target).length();
    distance *= (1.0f - delta * m_zoomSpeed);  // Adjust distance with zoom sensitivity

    QVector3D direction = (m_position - m_target).normalized();
    m_position = m_target + direction * distance;

    // Ensure distance remains within reasonable bounds
    float minDistance = 0.5f;
    float maxDistance = 100.0f;
    if (distance < minDistance) distance = minDistance;
    if (distance > maxDistance) distance = maxDistance;
}