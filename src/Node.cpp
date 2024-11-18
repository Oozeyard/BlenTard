#include "Node.h"

Node::Node(const QString& name, QObject* parent)
    : QObject(parent), m_name(name) 
{
    // Constructor implementation
}

Node::~Node() 
{
    delete m_model;
    qDeleteAll(m_children);
}

void Node::addChild(Node* child) 
{
    if (child && !m_children.contains(child)) {
        m_children.append(child);
        emit nodeChanged();
    }
}

void Node::removeChild(Node* child) 
{
    if (m_children.removeOne(child)) {
        emit nodeChanged();
    }
}

void Node::setName(const QString& name) 
{
    if (m_name != name) {
        m_name = name;
        emit nodeChanged();
    }
}