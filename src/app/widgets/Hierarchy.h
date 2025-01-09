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
    void keyPressed(QKeyEvent *event);
    void unselectNodes();

public slots:
    void updateNode(Node *node);
    void onItemClicked(QTreeWidgetItem *item);
    void selectNode(Node *node);

private:

    QTreeWidget *treeWidget;
    Node *rootNode { nullptr };
    QSet<Node *> selectedNodes;
    Node *m_currentNode { nullptr };

    void keyPressEvent(QKeyEvent *event) override; 
};
