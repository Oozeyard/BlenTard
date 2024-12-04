#pragma once

#include <QVector3D>

#include "Camera.h"

QVector3D rayPlaneIntersection(const QVector3D& rayOrigin, const QVector3D& rayDirection, const QVector3D& planeOrigin, const QVector3D& planeNormal);
void mouseToRay(const QPoint& mousePos, QVector3D& rayOrigin, QVector3D& rayDirection, const Camera* m_camera, const int width, const int height);
QVector3D worldToScreen(const QVector3D& worldPos, const Camera* m_camera, const int width, const int height);
inline uint qHash(const QVector3D& key, uint seed = 0) {
    return qHash(key.x(), seed) ^ qHash(key.y(), seed << 1) ^ qHash(key.z(), seed << 2);
}