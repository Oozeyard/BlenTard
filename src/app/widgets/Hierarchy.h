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

signals:
    void nodeSelected(Node *node);

public slots:
    void updateNode(Node *node);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void selectNode(Node *node);
private:

    QTreeWidget *treeWidget;
    Node *rootNode = nullptr;
    QSet<Node *> selectedNodes;
};
