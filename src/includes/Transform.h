#pragma once

#include <QMatrix4x4>
#include <QVector3D>

#include <cmath>

class Transform 
{
public:
    Transform() {};

    QVector3D position        { QVector3D(0.0f, 0.0f, 0.0f) };
    QVector3D rotationEuler   { QVector3D(0.0f, 0.0f, 0.0f) };
    QQuaternion rotation      { QQuaternion() };
    QVector3D scale           { QVector3D(1.0f, 1.0f, 1.0f) };

    QMatrix4x4 getModelMatrix() const {
        QMatrix4x4 modelMatrix;
        modelMatrix.QMatrix4x4::translate(position);
        modelMatrix.QMatrix4x4::rotate(rotation);
        modelMatrix.QMatrix4x4::scale(scale);
        return modelMatrix;
    };

    void translate(const QVector3D &translation) {
        position += translation;
    };

    void rotate(const QQuaternion &rotation) {
        this->rotation = rotation * this->rotation;
        this->rotationEuler = this->rotation.toEulerAngles();
    };

    void rotate(const QVector3D &euler) {
        this->rotationEuler += euler;
        this->rotationEuler.setX(fmod(this->rotationEuler.x(), 360.0f));
        this->rotationEuler.setY(fmod(this->rotationEuler.y(), 360.0f));
        this->rotationEuler.setZ(fmod(this->rotationEuler.z(), 360.0f));
        this->rotation = QQuaternion::fromEulerAngles(rotationEuler);
    };

    void scaleBy(const QVector3D &scale) {
        this->scale *= scale;
    };

    void setRotationEuler(const QVector3D &euler) {
        this->rotationEuler = euler;
        this->rotationEuler.setX(fmod(this->rotationEuler.x(), 360.0f));
        this->rotationEuler.setY(fmod(this->rotationEuler.y(), 360.0f));
        this->rotationEuler.setZ(fmod(this->rotationEuler.z(), 360.0f));
        this->rotation = QQuaternion::fromEulerAngles(rotationEuler);
    };
};
