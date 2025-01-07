#pragma once

#include <QMenu>
#include <QWidget>
#include <QContextMenuEvent>
#include <QAction>
#include <iostream>

enum ActionType {
    // Mesh
    ADD_CUBE,
    ADD_SPHERE,
    ADD_SUZANNE,
    ADD_CUSTOM_MODEL,

    // Light
    ADD_DIRECTIONAL_LIGHT,
    ADD_POINT_LIGHT,
    ADD_SPOT_LIGHT,

    UNIFY_MESH,
    SUBDIVIDE,
    EDGE_COLLAPSE_10,
    EDGE_COLLAPSE_20,
    EDGE_COLLAPSE_50,
    LAPLACIAN_SMOOTH,
    TAUBIN_SMOOTH,
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
