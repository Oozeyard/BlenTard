#pragma once

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtMath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <iostream>

// CONSTANTS
const QVector3D WORLD_UP = QVector3D(0.0f, 1.0f, 0.0f);

class Camera : public QObject
{
    Q_OBJECT
public:
    /* explicit */ Camera(QObject *parent = nullptr);
    Camera(QVector3D position, QVector3D target, QVector3D up, QObject *parent = nullptr);
    ~Camera();

    QMatrix4x4 getViewMatrix() const { return m_view; }
    QMatrix4x4 getProjectionMatrix() const { return m_projection; }

    void computeView(QMatrix4x4 &view, QMatrix4x4 &projection);

    float m_fov = 45.0f;
    float m_aspect = 16.0f / 9.0f;
    float m_near = 0.1f;
    float m_far = 100.0f;

    // Setters
    void setPosition(const QVector3D &position) { m_position = position; updateCamera(); }
    void setTarget(const QVector3D &target) { m_target = target; updateCamera(); }
    void setUpVector(const QVector3D &up) { m_up = up; }

    // Getters
    QVector3D getPosition() const { return m_position; }
    QVector3D getTarget() const { return m_target; }
    QVector3D getUpVector() const { return m_up; }
    QQuaternion getRotation() const { return m_rotation; }
    QVector3D getRotationEuler() const { return QVector3D(m_pitch, m_yaw, m_roll); }

    // Events
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    // Parameters
    QVector3D m_position;
    QVector3D m_target;
    QVector3D m_up;
    QQuaternion m_rotation;

    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    // Euler angles
    float m_yaw = 0;
    float m_pitch = 0;
    float m_roll = 0;

    QPoint m_lastMousePos;
    bool m_mousePressed = false;

    // Speed and sensibility
    float m_movementSpeed = 5.0f;
    float m_mouseSensitivity = 0.1f;
    float m_zoomSpeed = 0.5f;

    void updateCamera();

};