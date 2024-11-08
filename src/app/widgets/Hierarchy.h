#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

class Hierarchy : public QWidget
{
    Q_OBJECT

public:
    explicit Hierarchy(QWidget *parent = nullptr);

    void addObject(const QString &objectName); // Exemple pour ajouter des objets

private:
    QTreeWidget *treeWidget;
};
