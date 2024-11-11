#pragma once

#include <QMatrix4x4>
#include <QVector3D>

class Transform {
public:
    Transform() {};

    QVector3D position        { QVector3D(0.0f, 0.0f, 0.0f) };
    QVector3D rotationEuler   { QVector3D(0.0f, 0.0f, 0.0f) };
    QQuaternion rotation      { QQuaternion() };
    QVector3D scale           { QVector3D(1.0f, 1.0f, 1.0f) };
};
