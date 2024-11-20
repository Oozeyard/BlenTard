#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

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

private:
    QTreeWidget *treeWidget;
};
