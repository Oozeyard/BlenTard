#pragma once

#include <QMenu>
#include <QWidget>
#include <QContextMenuEvent>
#include <QAction>
#include <iostream>

class Context : public QMenu {
    Q_OBJECT

public:
    explicit Context(QWidget *parent = nullptr);
    void showContextMenu(const QPoint &pos);

signals:
    void actionTriggered(const QString &actionName);
};
