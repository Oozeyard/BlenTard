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

    void addObject(const QString &objectName); // Exemple pour ajouter des objets

public slots:
    void setRootNode(Node *node);

private:
    QTreeWidget *treeWidget;
};
