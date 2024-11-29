#include "Utils.h"

QVector3D rayPlaneIntersection(const QVector3D& rayOrigin, const QVector3D& rayDirection, const QVector3D& planeOrigin, const QVector3D& planeNormal) 
{
     float denominator = QVector3D::dotProduct(planeNormal, -rayDirection);
    if (qFuzzyCompare(denominator, 0.0f)) {
        // Ray is parallel to the plane
        return QVector3D(); // Return a default vector or handle the case appropriately
    }
    float t = QVector3D::dotProduct(planeNormal, rayOrigin - planeOrigin) / denominator;
    return rayOrigin + t * rayDirection;
}

void mouseToRay(const QPoint& mousePos, QVector3D& rayOrigin, QVector3D& rayDirection, const Camera* m_camera, const int width, const int height) 
{
    rayOrigin = m_camera->transform.position;

    float x = (2.0f * mousePos.x()) / width - 1.0f;
    float y = 1.0f - (2.0f * mousePos.y()) / height;
    float z = 1.0f;
    QVector3D ray_nds(x, y, z);

    QVector4D ray_clip(ray_nds.x(), ray_nds.y(), -1.0, 1.0);
    QVector4D ray_eye = m_camera->getProjectionMatrix().inverted() * ray_clip;
    ray_eye.setZ(-1.0);
    ray_eye.setW(0.0);

    QVector4D ray_world = m_camera->getViewMatrix().inverted() * ray_eye;
    rayDirection = ray_world.toVector3D().normalized();
}

QVector3D worldToScreen(const QVector3D& worldPos, const Camera* m_camera, const int width, const int height) 
{
    QMatrix4x4 modelView = m_camera->getViewMatrix();
    QMatrix4x4 projection = m_camera->getProjectionMatrix();

    QVector4D clipSpacePos = projection * modelView * QVector4D(worldPos, 1.0f);
    if (clipSpacePos.w() != 0.0f) {
        clipSpacePos /= clipSpacePos.w();
    }

    float x = (clipSpacePos.x() * 0.5f + 0.5f) * width;
    float y = (1.0f - (clipSpacePos.y() * 0.5f + 0.5f)) * height;
    return QVector3D(x, y, clipSpacePos.z());
}