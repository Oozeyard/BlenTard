#include "Context.h"

Context::Context(QWidget *parent) : QMenu(parent) {
    // Actions
    QAction *addCubeAction = addAction("Add Cube");
    QAction *addSphereAction = addAction("Add Sphere");
    QAction *addCustomAction = addAction("Add Custom Model");
    QAction *deleteAction = addAction("Delete");

    // Connexion des signaux pour gérer les actions
    connect(addCubeAction, &QAction::triggered, [this]() {
        emit actionTriggered("Add Cube");
    });
    connect(addSphereAction, &QAction::triggered, [this]() {
        emit actionTriggered("Add Sphere");
    });
    connect(addCustomAction, &QAction::triggered, [this]() {
        emit actionTriggered("Add Custom Model");
    });
    connect(deleteAction, &QAction::triggered, [this]() {
        emit actionTriggered("Delete");
    });
}

void Context::showContextMenu(const QPoint &pos) {
    exec(pos); // Show the context menu
}
