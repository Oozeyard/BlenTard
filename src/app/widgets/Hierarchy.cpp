#include "Hierarchy.h"

Hierarchy::Hierarchy(QWidget *parent) 
    : QWidget(parent), 
      treeWidget(new QTreeWidget(this)) {

    treeWidget->setHeaderLabel("Scene Hierarchy");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(treeWidget);
    setLayout(layout);

    connect(treeWidget, &QTreeWidget::itemClicked, this, &Hierarchy::onItemClicked);
}

void Hierarchy::updateNode(Node *node)
{
    if (!node) return;

    rootNode = node;
    treeWidget->clear(); // clear tree

    // add nodes
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

    // store node in item
    currentItem->setData(0, Qt::UserRole, QVariant::fromValue(static_cast<void *>(node)));

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

void Hierarchy::onItemClicked(QTreeWidgetItem *item, int column) {
    if (!item) return;

    // retrieve node
    Node *node = static_cast<Node *>(item->data(0, Qt::UserRole).value<void *>());
    if (!node) return;

    // Ctrl handling
    // if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
    //     if (selectedNodes.contains(node)) {
    //         node->setSelected(); // unselect
    //         selectedNodes.remove(node);
    //     } else {
    //         node->setSelected(); // select
    //         selectedNodes.insert(node);
    //     }
    // } else { // single selection
    //     for (Node *selectedNode : selectedNodes) {
    //         selectedNode->setSelected();
    //     }
    //     selectedNodes.clear();

    //     // select new node
    //     node->setSelected();
    //     selectedNodes.insert(node);
    // }

    emit unselectNodes();
    node->setSelected();
    emit nodeSelected(node);
}

void Hierarchy::selectNode(Node *node) {
    emit nodeSelected(node);
}

void Hierarchy::keyPressEvent(QKeyEvent *event) {
    emit keyPressed(event);
}