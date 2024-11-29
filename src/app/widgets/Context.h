#pragma once

#include <QMenu>
#include <QWidget>
#include <QContextMenuEvent>
#include <QAction>
#include <iostream>

enum ActionType {
    ADD_CUBE,
    ADD_SPHERE,
    ADD_CUSTOM_MODEL,
    SUBDIVIDE,
    DELETE
};

class Context : public QMenu {
    Q_OBJECT

public:
    explicit Context(QWidget *parent = nullptr);
    void showContextMenu(const QPoint &pos);

signals:
    void actionTriggered(const ActionType &actionType);
};
