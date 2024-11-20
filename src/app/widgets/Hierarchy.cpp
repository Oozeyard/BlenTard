#include "Hierarchy.h"

Hierarchy::Hierarchy(QWidget *parent) 
    : QWidget(parent), 
      treeWidget(new QTreeWidget(this)) {

    treeWidget->setHeaderLabel("Scene Hierarchy");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(treeWidget);
    setLayout(layout);    
}

void Hierarchy::addObject(const QString &objectName)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget);
    item->setText(0, objectName);
}

void Hierarchy::setRootNode(Node *node)
{
    if (node) {
        for (Node* child : node->getChildren()) {
            addObject(child->getName());
            setRootNode(child);
        }
    }
}