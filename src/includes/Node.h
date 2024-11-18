#pragma once

#include <QObject>
#include <QVector>
#include <QString>

#include "Transform.h"
#include "Model.h"

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

    void setModel(Model *model) { m_model = model; }
    Model* getModel() const { return m_model; }

signals:
    void nodeChanged();

private:
    QString m_name;
    QVector<Node*> m_children;
    Model *m_model;
};