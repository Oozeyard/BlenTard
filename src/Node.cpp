#include "Node.h"

Node::Node(const QString& name, QObject* parent)
    : QObject(parent), m_name(name) 
{
    // Constructor implementation
}

Node::~Node() 
{
    for (Node* child : m_children) {
        delete child; // Libère la mémoire des enfants
    }
}


void Node::addChild(Node* child) 
{
    if (child && !m_children.contains(child)) {
        m_children.append(child);
        child->setParent(this);
        emit nodeChanged();
    }
}

void Node::removeChild(Node* child) 
{
    if (m_children.removeOne(child)) {
        child->setParent(nullptr);
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


void Node::draw(QOpenGLShaderProgram *program) 
{
    if (!program) {
        return;
    }

    // Set up the shader program
    // program->bind();

    // Draw children nodes
    for (Node* child : m_children) {
        child->draw(program);
    }

    // program->release();
}

void Node::setSelected() {
    m_selected = !m_selected;
    for (Node* child : m_children) {
        child->setSelected();
    }
}


void Node::deleteSelectedNodes() {
    for (Node* child : m_children) {
        if (child->isSelected()) {
            removeChild(child);
        } else {
            child->deleteSelectedNodes();
        }
    }
}