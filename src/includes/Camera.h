#pragma once

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtMath>

#include <glm/glm.hpp>

#include <iostream>


// CONSTANTS
const QVector3D WORLD_UP = QVector3D(0.0f, 1.0f, 0.0f);

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);
    Camera(QVector3D position, QVector3D target, QObject *parent = nullptr);

    void computeView(QMatrix4x4 &view, QMatrix4x4 &projection);

    // Setters
    void setPosition(const QVector3D &position) { m_position = position; }
    void setTarget(const QVector3D &target) { m_target = target; }
    void setFront(const QVector3D &front) { m_front = front; }
    void setFov(float fov) { m_fov = fov; }
    void setAspect(float aspect) { m_aspect = aspect; }
    void setNear(float near) { m_near = near; }
    void setFar(float far) { m_far = far; }
    void setRotation(const QQuaternion &rotation) { m_rotation = rotation; }
    void setRotationEuler(const QVector3D &euler) { m_euler = euler; }


    // Getters
    QVector3D getPosition() const { return m_position; }
    QVector3D getTarget() const { return m_target; }
    QVector3D getFront() const { return m_front; }
    float getFov() const { return m_fov; }
    float getAspect() const { return m_aspect; }
    float getNear() const { return m_near; }
    float getFar() const { return m_far; }
    QQuaternion getRotation() const { return m_rotation; }
    QVector3D getRotationEuler() const { return m_euler; }
    QMatrix4x4 getViewMatrix() const { return m_viewMatrix; }
    QMatrix4x4 getProjectionMatrix() const { return m_projectionMatrix; }

    // Events
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void init();

    // Parameters
    QVector3D m_position { QVector3D(0.0f, 5.0f, 10.0f) };
    QVector3D m_target   { QVector3D(0.0f, 0.0f, 0.0f) };
    QVector3D m_front    { QVector3D(0.0f, 0.0f, -1.0f) };
    float m_fov     { 45.0f };
    float m_aspect  { 16.0f / 9.0f };
    float m_near    { 0.1f };
    float m_far     { 100.0f };

    // Rotation 
    QQuaternion m_rotation { QQuaternion() };

    // Euler angles
    QVector3D m_euler { QVector3D(0.0f, 0.0f, 0.0f) };

    // View
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;

    QPoint m_lastMousePos;
    bool m_mousePressed = false;

    // Speed and sensibility
    float m_movementSpeed       { 0.01f };
    float m_mouseSensitivity    { 0.05f };
    float m_zoomSpeed           { 0.05f };

};