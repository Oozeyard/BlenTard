#pragma once

#include <QObject>
#include <QVector>
#include <QString>

#include "Transform.h"

class Node : public QObject 
{
    Q_OBJECT

public:
    explicit Node(const QString& name, QObject* parent = nullptr);
    ~Node();

    Transform transform; // Each node have a transform

    void addChild(Node* child);
    void removeChild(Node* child);
    QVector<Node*> getChildren() const;

    QString getName() const;
    void setName(const QString& name);

signals:
    void nodeChanged();

private:
    QString m_name;
    QVector<Node*> m_children;

};