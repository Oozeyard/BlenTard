#include "Context.h"

Context::Context(QWidget *parent) : QMenu(parent) {

    // Actions
    QMenu *meshMenu = addMenu("Add Mesh");
    QAction *addCubeAction = meshMenu->addAction("Cube");
    QAction *addSphereAction = meshMenu->addAction("Sphere");
    QAction *addSuzanneAction = meshMenu->addAction("Monkey :)");
    QAction *addCustomAction = meshMenu->addAction("Custom Model");

    addSeparator();
    QMenu *LightMenu = addMenu("Add Light");
    QAction *addDirectionalLightAction = LightMenu->addAction("Directional Light");
    QAction *addPointLightAction = LightMenu->addAction("Point Light");
    QAction *addSpotLightAction = LightMenu->addAction("Spot Light");

    addSeparator();
    QAction *unifyMeshAction = addAction("Unify Mesh");

    addSeparator();
    QMenu *SubdivideMenu = addMenu("Subdivide");
    QAction *subdivideAction = SubdivideMenu->addAction("Default");
    QAction *subdivideLoopAction = SubdivideMenu->addAction("Loop");
    // QAction *subdivideCatmullClarkAction = SubdivideMenu->addAction("Catmull-Clark");

    QMenu *SmoothMenu = addMenu("Smooth");
    QAction *LaplacianSmoothAction = SmoothMenu->addAction("Laplacian");
    QAction *TaubinSmoothAction = SmoothMenu->addAction("Taubin");

    addSeparator();

    QAction *deleteAction = addAction("Delete");

    // Connect actions

    // Mesh
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

    // Light
    connect(addDirectionalLightAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_DIRECTIONAL_LIGHT);
    });
    connect(addPointLightAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_POINT_LIGHT);
    });
    connect(addSpotLightAction, &QAction::triggered, [this]() {
        emit actionTriggered(ADD_SPOT_LIGHT);
    });

    connect(unifyMeshAction, &QAction::triggered, [this]() {
        emit actionTriggered(UNIFY_MESH);
    });
    connect(subdivideAction, &QAction::triggered, [this]() {
        emit actionTriggered(SUBDIVIDE);
    });
    connect(subdivideLoopAction, &QAction::triggered, [this]() {
        emit actionTriggered(SUBDIVIDELOOP);
    });
    // connect(subdivideCatmullClarkAction, &QAction::triggered, [this]() {
    //     emit actionTriggered(SUBDIVIDECATMULLCLARK);
    // });
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
