#include "Camera.h"

Camera::Camera(QObject *parent)
    : QObject(parent)
{
    init();
}

Camera::Camera(QVector3D position, QVector3D target, QObject *parent)
    : QObject(parent),
      m_position(position),
      m_target(target)
{
    init();
}

void Camera::init()
{
    QVector3D direction = (m_target - m_position).normalized();
    m_euler.setX(qRadiansToDegrees(atan2(direction.y(), direction.z())));
    m_euler.setY(qRadiansToDegrees(atan2(direction.x(), direction.z())));
    m_euler.setZ(0.0f);
    m_rotation = QQuaternion::fromEulerAngles(m_euler);
}

void Camera::computeView(QMatrix4x4 &view, QMatrix4x4 &projection) 
{ 
    projection.setToIdentity();  
    projection.perspective(m_fov, m_aspect, m_near, m_far);
    m_projectionMatrix = projection;

    view.setToIdentity();
    view.lookAt(m_position, m_target, WORLD_UP);
    m_viewMatrix = view; 
}


void Camera::keyPressEvent(QKeyEvent *event)
{
    std::cout << "Key pressed: " << event->key() << std::endl;

}

void Camera::mousePressEvent(QMouseEvent *event)
{   
    m_lastMousePos = event->pos();
    m_mousePressed = true;
}

void Camera::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mousePressed && event->buttons() & Qt::MiddleButton && event->modifiers() & Qt::ShiftModifier) {
        QPoint delta = event->pos() - m_lastMousePos;
        float dx = delta.x() * m_movementSpeed;
        float dy = delta.y() * m_movementSpeed;

        QVector3D right = QVector3D::crossProduct(WORLD_UP, m_target - m_position).normalized();
        QVector3D up = QVector3D::crossProduct(m_target - m_position, right).normalized();

        m_position += right * dx + up * dy;
        m_target += right * dx + up * dy;

        m_lastMousePos = event->pos();
    }

    if (m_mousePressed && event->buttons() & Qt::MiddleButton && !(event->modifiers() & Qt::ShiftModifier)) {
        QPoint delta = event->pos() - m_lastMousePos;
        float dx = -delta.x() * m_mouseSensitivity;
        float dy = delta.y() * m_mouseSensitivity;

        QQuaternion rotationX = QQuaternion::fromAxisAndAngle(WORLD_UP, dx);
        QVector3D right = QVector3D::crossProduct(WORLD_UP, m_target - m_position).normalized();
        QQuaternion rotationY = QQuaternion::fromAxisAndAngle(right, dy);

        m_rotation = rotationX * rotationY;
        m_euler = m_rotation.toEulerAngles();
        m_position = m_target + m_rotation.rotatedVector(m_position - m_target);

        m_lastMousePos = event->pos();
    }


}

void Camera::wheelEvent(QWheelEvent *event)
{
    float delta = glm::clamp((float)event->angleDelta().y(), -120.0f, 120.0f) / 120.0f; // Normalize to -1, 0, or 1
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