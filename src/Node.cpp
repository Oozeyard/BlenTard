#include "Node.h"

uint Node::s_nextID = 1;
std::unordered_map<uint, Node*> Node::s_nodeRegistry; 

Node::Node(const QString& name, QObject* parent)
    : QObject(parent), m_name(name), m_id(s_nextID++)
{
    s_nodeRegistry[m_id] = this;
}

Node::~Node() 
{
    s_nodeRegistry.erase(m_id);
    for (Node* child : m_children) {
        delete child; // Recursively delete all children
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

void Node::deselectAll() {
    m_selected = false;
    for (Node* child : m_children) {
        child->deselectAll();
    }
}

void Node::selectAll() {
    m_selected = true;
    for (Node* child : m_children) {
        child->selectAll();
    }
}

void Node::deleteSelectedNodes() {
    for (Node* child : m_children) {
        if (child->isSelected()) {
            removeChild(child);
        } 
        child->deleteSelectedNodes();
    }
}

void Node::setEditMode() {
    m_editMode = !m_editMode;
    for (Node* child : m_children) {
        child->setEditMode();
    }
}

void Node::disableAllEditMode() {
    m_editMode = false;
    for (Node* child : m_children) {
        child->disableAllEditMode();
    }
}

Node* Node::getNodeById(uint id) {
    auto it = s_nodeRegistry.find(id);
    if (it != s_nodeRegistry.end()) {
        return it->second;
    }
    return nullptr; // Not found
}

QVector<Node*> Node::getSelectedNode() {
    QVector<Node*> selectedNodes;
    for (Node* child : m_children) {
        if (child->isSelected()) {
            selectedNodes.append(child);
        }
    }
    return selectedNodes;
}

QVector3D idToColor(int id) {
    return QVector3D(
        ((id & 0xFF)) / 255.0f,           // Red
        ((id >> 8) & 0xFF) / 255.0f,      // Green
        ((id >> 16) & 0xFF) / 255.0f      // Blue
    );
}

int colorToID(unsigned char r, unsigned char g, unsigned char b) {
    return ((r) + ((g) << 8) + ((b) << 16));
}
