#include "Context.h"

Context::Context(QWidget *parent) : QMenu(parent) {

    // Actions
    QMenu *meshMenu = addMenu("Add Mesh");
    QAction *addCubeAction = meshMenu->addAction("Cube");
    QAction *addSphereAction = meshMenu->addAction("Sphere");
    QAction *addSuzanneAction = meshMenu->addAction("Monkey :)");
    QAction *addCustomAction = meshMenu->addAction("Custom Model");

    addSeparator();
    QAction *subdivideAction = addAction("Subdivide");
    QMenu *SmoothMenu = addMenu("Smooth");
    QAction *LaplacianSmoothAction = SmoothMenu->addAction("Laplacian");
    QAction *TaubinSmoothAction = SmoothMenu->addAction("Taubin");

    addSeparator();

    QAction *deleteAction = addAction("Delete");

    // Connexion des signaux pour gérer les actions
    connect(addCubeAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_CUBE);
    });
    connect(addSphereAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_SPHERE);
    });
    connect(addSuzanneAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_SUZANNE);
    });
    connect(addCustomAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_CUSTOM_MODEL);
    });
    connect(subdivideAction, &QAction::triggered, [this]() {
        emit actionTriggered(SUBDIVIDE);
    });
    connect(LaplacianSmoothAction, &QAction::triggered, [this]() {
        emit actionTriggered(LAPLACIAN_SMOOTH);
    });
    connect(TaubinSmoothAction, &QAction::triggered, [this]() {
        emit actionTriggered(TAUBIN_SMOOTH);
    });
    connect(deleteAction, &QAction::triggered, [this]() {
        emit actionTriggered(DELETE);
    });
}

void Context::showContextMenu(const QPoint &pos) {
    exec(pos); // Show the context menu
}
