#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QOpenGLShaderProgram>

#include "Transform.h"

#include <iostream>

class Node : public QObject 
{
    Q_OBJECT

public:
    explicit Node(const QString& name, QObject* parent = nullptr);
    ~Node();

    Transform transform; // Each node have a transform

    void addChild(Node* child);
    void removeChild(Node* child);
    QVector<Node*> getChildren() const { return m_children; }

    QString getName() const { return m_name; }
    void setName(const QString& name);

    virtual void draw(QOpenGLShaderProgram *program);

    void setSelected();
    bool isSelected() const { return m_selected; };

    void deleteSelectedNodes();

signals:
    void nodeChanged();

protected:
    QString m_name;
    QVector<Node*> m_children;
    bool m_selected = false;
};