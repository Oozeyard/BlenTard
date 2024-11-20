#include "Hierarchy.h"

Hierarchy::Hierarchy(QWidget *parent) 
    : QWidget(parent), 
      treeWidget(new QTreeWidget(this)) {

    treeWidget->setHeaderLabel("Scene Hierarchy");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(treeWidget);
    setLayout(layout);    
}

void Hierarchy::setRootNode(Node *node)
{
    if (!node) return;

    // clear tree
    treeWidget->clear();

    // add node
    for (Node *child : node->getChildren()) {
        addNode(child, nullptr);
    }
}

void Hierarchy::addNode(Node *node, QTreeWidgetItem *parentItem)
{
    if (!node) return;

    // create node
    QTreeWidgetItem *currentItem = new QTreeWidgetItem();
    currentItem->setText(0, node->getName());

    if (parentItem) {
        parentItem->addChild(currentItem); // add to parent
    } else {
        treeWidget->addTopLevelItem(currentItem); // add as root
    }

    // check children
    for (Node *child : node->getChildren()) {
        addNode(child, currentItem);
    }
}
