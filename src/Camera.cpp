#include "Camera.h"

Camera::Camera(const QString& name, Node *parent) : 
    Node(name, parent)
{
    transform.position = QVector3D(5.0f, -10.0f, 8.0f);
    init();
    computeView(m_viewMatrix, m_projectionMatrix);
}

Camera::Camera(const QString& name, QVector3D position, QVector3D target, Node *parent) : 
    Node(name, parent),
    m_target(target)
{
    init();
    transform.position = position;
    computeView(m_viewMatrix, m_projectionMatrix);

}

void Camera::init()
{
    QVector3D direction = (m_target - transform.position).normalized();
    transform.rotationEuler.setX(qRadiansToDegrees(atan2(direction.y(), direction.z())));
    transform.rotationEuler.setY(qRadiansToDegrees(atan2(direction.x(), direction.z())));
    transform.rotationEuler.setZ(0.0f);
    transform.rotation = QQuaternion::fromEulerAngles(transform.rotationEuler);
}

void Camera::computeView(QMatrix4x4 &view, QMatrix4x4 &projection) 
{ 
    projection.setToIdentity();  

    if (m_isOrthographic) {
        float distanceToTarget = (transform.position - m_target).length();
        float orthoSize = tan((m_fov / 2.0f) * M_PI / 180.0f) * distanceToTarget;
        projection.ortho(-orthoSize * m_aspect, orthoSize * m_aspect, -orthoSize, orthoSize, m_near, m_far);
    }
    else  {
        projection.perspective(m_fov, m_aspect, m_near, m_far);
    }
    m_projectionMatrix = projection;

    view.setToIdentity();
    view.lookAt(transform.position, m_target, WORLD_UP);
    m_viewMatrix = view; 

    m_front = (m_target - transform.position).normalized();
}


void Camera::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == 53) { // NUMPAD_5
        std::cout << "Ortho" << std::endl;
        m_isOrthographic = !m_isOrthographic;
        computeView(m_viewMatrix, m_projectionMatrix);
    }

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

        QVector3D right = QVector3D::crossProduct(WORLD_UP, m_target - transform.position).normalized();
        QVector3D up = QVector3D::crossProduct(m_target - transform.position, right).normalized();

        transform.position += right * dx + up * dy;
        m_target += right * dx + up * dy;

        m_lastMousePos = event->pos();
    }

    if (m_mousePressed && event->buttons() & Qt::MiddleButton && !(event->modifiers() & Qt::ShiftModifier)) {
        QPoint delta = event->pos() - m_lastMousePos;
        float dx = -delta.x() * m_mouseSensitivity;
        float dy = delta.y() * m_mouseSensitivity;

        QQuaternion rotationX = QQuaternion::fromAxisAndAngle(WORLD_UP, dx);
        QVector3D right = QVector3D::crossProduct(WORLD_UP, m_target - transform.position).normalized();
        QQuaternion rotationY = QQuaternion::fromAxisAndAngle(right, dy);

        transform.rotation = rotationX * rotationY;
        transform.rotationEuler = transform.rotation.toEulerAngles();
        transform.position = m_target + transform.rotation.rotatedVector(transform.position - m_target);

        m_lastMousePos = event->pos();
    }

    computeView(m_viewMatrix, m_projectionMatrix);

}

void Camera::wheelEvent(QWheelEvent *event)
{
    float delta = qBound(-120.0f, (float)event->angleDelta().y(), 120.0f) / 120.0f; // Normalize to -1, 0, or 1
    float distance = (transform.position - m_target).length();
    distance *= (1.0f - delta * m_zoomSpeed);  // Adjust distance with zoom sensitivity

    QVector3D direction = (transform.position - m_target).normalized();
    transform.position = m_target + direction * distance;

    // Ensure distance remains within reasonable bounds
    float minDistance = 0.5f;
    float maxDistance = 100.0f;
    if (distance < minDistance) distance = minDistance;
    if (distance > maxDistance) distance = maxDistance;

    computeView(m_viewMatrix, m_projectionMatrix);
}