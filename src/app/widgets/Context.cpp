#include "Context.h"

Context::Context(QWidget *parent) : QMenu(parent) {
    // Actions
    QAction *addCubeAction = addAction("Add Cube");
    QAction *addSphereAction = addAction("Add Sphere");
    QAction *addCustomAction = addAction("Add Custom Model");
    QAction *subdivideAction = addAction("Subdivide");
    QAction *deleteAction = addAction("Delete");

    // Connexion des signaux pour gérer les actions
    connect(addCubeAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_CUBE);
    });
    connect(addSphereAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_SPHERE);
    });
    connect(addCustomAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_CUSTOM_MODEL);
    });
    connect(subdivideAction, &QAction::triggered, [this]() {
        emit actionTriggered(SUBDIVIDE);
    });
    connect(deleteAction, &QAction::triggered, [this]() {
        emit actionTriggered(DELETE);
    });
}

void Context::showContextMenu(const QPoint &pos) {
    exec(pos); // Show the context menu
}
