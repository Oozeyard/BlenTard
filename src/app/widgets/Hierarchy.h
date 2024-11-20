#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QSet>
#include <QApplication>

#include "Node.h"

#include <iostream>

class Hierarchy : public QWidget
{
    Q_OBJECT

public:
    explicit Hierarchy(QWidget *parent = nullptr);
    void addNode(Node *node, QTreeWidgetItem *parentItem);

public slots:
    void setRootNode(Node *node);
    void onItemClicked(QTreeWidgetItem *item, int column);
private:

    QTreeWidget *treeWidget;
    Node *rootNode = nullptr;
    Node *currentSelectedNode = nullptr;
    QSet<Node *> selectedNodes;
};
