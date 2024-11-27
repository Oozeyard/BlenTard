#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QOpenGLShaderProgram>

#include "Transform.h"

#include <iostream>
#include <unordered_map>

class Node : public QObject 
{
    Q_OBJECT

public:
    explicit Node(const QString& name, QObject* parent = nullptr);
    ~Node();

    Transform transform; // Each node have a transform
    Transform& getTransform() { return transform; }

    void addChild(Node* child);
    void removeChild(Node* child);
    QVector<Node*> getChildren() const { return m_children; }

    QString getName() const { return m_name; }
    void setName(const QString& name);

    virtual void draw(QOpenGLShaderProgram *program);

    void setSelected();
    bool isSelected() const { return m_selected; };

    void deleteSelectedNodes();
    void deselectAll();
    void selectAll();
    QVector<Node*> getSelectedNode();

    uint getId() const { return m_id; }
    static Node* getNodeById(uint id);

signals:
    void nodeChanged();

protected:
    QString m_name;
    QVector<Node*> m_children;
    bool m_selected = false;

private:
    uint m_id;

    static uint s_nextID;
    static std::unordered_map<uint, Node*> s_nodeRegistry;
};

QVector3D idToColor(int id);
int colorToID(unsigned char r, unsigned char g, unsigned char b);