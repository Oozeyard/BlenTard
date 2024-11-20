#pragma once

#include <QObject>
#include <QTreeWidget>

class Select : public QObject {
    Q_OBJECT

public:
    explicit Select(QObject *parent = nullptr);

public slots:
    //void onNodeSelected(QTreeWidgetItem *item);

private:
};